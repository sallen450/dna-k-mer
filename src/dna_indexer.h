#ifndef DNA_INDEXER_H
#define DNA_INDEXER_H
#include <cstring>
#include <cstdint>
#include <map>


const uint32_t DEFAULT_BUFFER_SIZE  = 1024;
const uint32_t UINT_32_MAX          = 0xFFFFFFFF;

struct Position
{
    uint32_t pos;
    int count;
    struct Position* next;
};

class dna_indexer
{
public:
    dna_indexer(const uint32_t pattern_length, const uint32_t dna_count, const uint32_t dna_length, const uint32_t hash_table_size = 0);
    void add(const char* dna_string);
    void export_dna_pattern(const uint32_t position, const uint32_t length, std::string& dna_buff) const;
    void build_index();
    void search(const char *dna_pattern_string);
    bool is_full() const;
    virtual ~dna_indexer();
    
    std::map<std::string, int*> m_position_array;
    std::map<std::string, int> m_position_count;
    uint32_t    m_hash_table_size;
    
protected:
    struct Position* get_hash_table_slot(std::string& dna_buff, const uint32_t logical_address = UINT_32_MAX);
    
private:
    uint32_t    m_pattern_length;
    uint32_t    m_dna_count;
    uint32_t    m_dna_length;
    
    uint8_t*    m_data;
    uint32_t    m_data_length;

    std::map<std::string, struct Position*> m_hash_table;
    uint32_t    m_pattern_buffer_size;
    uint8_t     m_dna_2_byte[128];
    
    
    //DEBUG
//    int m_hash_table_count;
};

#endif // DNA_INDEXER_H
