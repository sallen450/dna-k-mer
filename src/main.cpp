#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cmath>
#if defined (__linux__)
#include <sys/time.h>
#else
#include <ctime>
#endif

#include "dna_indexer.h"

using namespace std;

#if defined (__linux__)
struct timeval start_time, last_time;
#else
clock_t start_time, last_time;
#endif

void log(const char *format, ...)
{
#if defined (__linux__)
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
#else
    clock_t current_time = clock();
#endif
    printf(
           "%8.3f %8.3f | ",
#if defined (__linux__)
           current_time.tv_sec - start_time.tv_sec + (current_time.tv_usec - start_time.tv_usec) / 1e6 ,
           current_time.tv_sec - last_time.tv_sec + (current_time.tv_usec - last_time.tv_usec) / 1e6
#else
           (current_time - start_time) / (double) CLOCKS_PER_SEC, (current_time - last_time) / (double) CLOCKS_PER_SEC
#endif
           );
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
    
#if defined (__linux__)
    gettimeofday(&last_time, NULL);
#else
    last_time = clock();
#endif
}



int main()
{
#if defined (__linux__)
    gettimeofday(&start_time, NULL);
    gettimeofday(&last_time, NULL);
#else
    start_time = last_time = clock();
#endif
    
    log("start\n");
    
    int         k               = 50;
    int         dna_count       = 1000000;
//    int         dna_count       = 10;     //debug
    int         dna_length      = 100;
    int         hash_table_size = std::pow(4, k);       // hash_table_size > power(4, m_pattern_length);
    
    dna_indexer* indexers = new dna_indexer(k, dna_count, dna_length, hash_table_size);
    
    const char  *file_name      = "/yourFilePath/100w";
    if (file_name == NULL) {
        log("file not found.\n");
        delete indexers;
        system("pause");
        return 1;
    }
    
    
    log("allocate indexer memory\n");
    
    FILE *file = fopen(file_name, "r");
    char buffer[1024];
    while(fgets(buffer, 1024, file))
    {
        
        if(buffer[0] == '\0') break;
        
        if(indexers->is_full())
        {
            log("data read done!");
            break;
        }
        
        indexers->add(buffer);
    }
    fclose(file);
    
    log("%d dna was read.\n", dna_count);
    
    
    indexers->build_index();
    
    
    /* ************* */
    float array_and_count_map_container_size = (sizeof(indexers->m_position_array) + sizeof(indexers->m_position_count)) /(float)(std::pow(2, 30));
    
    float all_posotion_array_size = (4 * (dna_count * (dna_length -k + 1))) / (float)(pow(2, 30));
    std::cout << "k = " << k << "  " << "m_hash_table : " << array_and_count_map_container_size << "GB" << endl;
    std::cout << "k = " << k << "  " << "all_posotion_array_size : " << all_posotion_array_size << "GB" << std::endl;
    std::cout << "k = " << k << "  " << "total_size : " << all_posotion_array_size + array_and_count_map_container_size << "GB" << std::endl;
    std::cout << "************************************************" << std::endl;
    /* ************* */
    log("indexers build is done, input to search.\n");
    
    while(true)
    {
        log("please input: ");
        if(!gets(buffer) || buffer[0] == '\0') break;
        log("got    input: %s\n", buffer);
        
        indexers->search(buffer);
    }
    
    delete indexers;
    log("program terminated.\n");
    return 0;
}

