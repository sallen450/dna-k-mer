#include <iostream>
#include "dna_indexer.h"


dna_indexer::dna_indexer(const uint32_t pattern_length, const uint32_t dna_count, const uint32_t dna_length, const uint32_t hash_table_size)
{
    m_pattern_length    = pattern_length;
    m_dna_count        = dna_count;
    m_dna_length       = dna_length;
    
    m_hash_table_size   = hash_table_size;
    

    int max_data_length = dna_count * dna_length;
    
    m_data = new uint8_t[max_data_length];
    memset(m_data, 0, max_data_length);
    m_data_length = 0;
    
    m_pattern_buffer_size       = m_pattern_length;
    
    
    memset(m_dna_2_byte, -1, sizeof(m_dna_2_byte));
    m_dna_2_byte['A'] = 0;
    m_dna_2_byte['T'] = 1;
    m_dna_2_byte['C'] = 2;
    m_dna_2_byte['G'] = 3;
    
    //debug
//    m_hash_table_count = 0;
}

dna_indexer::~dna_indexer()
{
    std::map<std::string, int*>::iterator it;
    for(it=m_position_array.begin();it!=m_position_array.end();it++)
    {
        int* p = it->second;
        delete [] p;
    }


    delete m_data;
}


void dna_indexer::add(const char* dna_string)
{
    for(int i = 0; dna_string[i]; i++)
    {
        uint8_t x = m_dna_2_byte[(uint8_t)dna_string[i]];
        if(x == 0xFF)continue;
        
        m_data[m_data_length] = dna_string[i];
        
        m_data_length++;
    }
}
bool dna_indexer::is_full() const
{
    return m_data_length == m_dna_count * m_dna_length;
}

struct Position* dna_indexer::get_hash_table_slot(std::string& dna_buff, const uint32_t position)
{
    if(position != UINT_32_MAX)
    {
        struct Position* sp_pos = (struct Position*)malloc(sizeof(struct Position));
        sp_pos->pos = position;
        sp_pos->next = m_hash_table[dna_buff];
        if (sp_pos->next == NULL) {
            //debug
//            m_hash_table_count++;
            //debug
            sp_pos->count = 1;
        }
        else {
            sp_pos->count = sp_pos->next->count + 1;
        }
        
        //debug
//        std::cout << dna_buff << std::endl;
//        std::cout << "sp_pos->count: " << sp_pos->count << std::endl;
//        std::cout << "sp_pos->pos: " << sp_pos->pos << std::endl;
        //debug
        
        m_hash_table[dna_buff] = sp_pos;
        
        m_position_array[dna_buff] = NULL;
        m_position_count[dna_buff] = 0;
    }
    return m_hash_table[dna_buff];
}

void dna_indexer::export_dna_pattern(const uint32_t position, const uint32_t length, std::string& dna_buff) const
{
    for(uint32_t i = 0; i < length; i++)
    {
        dna_buff += m_data[position + i];
    }
}


void dna_indexer::build_index()
{
    uint32_t position = 0;
    
    for(uint32_t i = 0; i < m_data_length / m_dna_length; i++)
    {
        for(uint32_t j = 0; j < m_dna_length - m_pattern_length + 1; j++)
        {
            position = i * m_dna_length + j;
            std::string str = "";
            export_dna_pattern(position, m_pattern_length, str);
            get_hash_table_slot(str, position);
        }
        
        //debug
        if (i % 10000 == 0) {
            std::cout << "    已经处理到" << i << "行" << std::endl;
        }
        //debug
    }
    
    //debug
//    int kkk = 0;
//    std::cout << "m_hash_table_count : " << m_hash_table_count << std::endl;

    //debug
    
    //debug
    unsigned long dealed_memory_count = 1;
    unsigned long total_memory_count = m_hash_table.size();
    //debug
    
    std::map<std::string, struct Position*>::iterator it;
    for(it=m_hash_table.begin();it!=m_hash_table.end();)
    {
        
        struct Position* p = it->second;
        int position_count = it->second->count;
        
        //debug
//        std::cout << it->first << std::endl;
        //debug
        
        m_position_count[it->first] = position_count;
        
        int* pa = new int[position_count];
        
        for (int i = 0; i < position_count; ++i) {
            struct Position* del = p;
            
            pa[i] = p->pos;
            
            p = p->next;
            delete del;
        }
        
        m_position_array[it->first] = pa;
        
        m_hash_table.erase(it++);
        
        //debug
        if (dealed_memory_count % 10 == 0)
        {
            std::cout << "内存压缩进度 : " << dealed_memory_count << " / " << total_memory_count << std::endl;
        }
        dealed_memory_count++;
        
        //debug
        
        //debug
//        kkk++;
//        std::cout << "kkk : " << kkk << std::endl;
        //debug
    }
}

void dna_indexer::search(const char *dna_pattern_string)
{
    std::string str = "";
    for (int i = 0; i < m_pattern_length; ++i) {
        str += dna_pattern_string[i];
    }
    
    int* p = m_position_array[str];

    if (p == NULL) {
        std::cout << "it was not found." << std::endl;
        return;
    }
    
    int count = m_position_count[str];
    
    for (int i = 0 ; i < count; ++i) {
        std::cout << "it is at " << (p[i] / m_dna_length + 1) << "\t" << (p[i] % m_dna_length + 1) << std::endl;
    }
    
}


