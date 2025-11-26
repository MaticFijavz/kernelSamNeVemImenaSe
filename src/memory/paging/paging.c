#include "memory/paging/paging.h"
#include "memory/heap/kheap.h"
#include <stdint.h>

void paging_load_directory(uint32_t* directory); 

static uint32_t* current_directory = 0;
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags){
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    uint32_t offset = 0;
    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; ++i){
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for(int b = 0; b <PAGING_TOTAL_ENTRIES_PER_TABLE; ++b){
            entry[b] = (offset + (b * PAGING_PAGE_SIZE)) | flags;
        }
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}

int paging_get_indexes(void* virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out)
{
    if ((uint32_t)virtual_address % PAGING_PAGE_SIZE != 0)
    {
        return -1;
    }

    *directory_index_out = (uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    *table_index_out = ((uint32_t)virtual_address / PAGING_PAGE_SIZE) % PAGING_TOTAL_ENTRIES_PER_TABLE;

    return 0;
}

void paging_map_to(struct paging_4gb_chunk* directory, void* virt, void* phys, int flags)
{
    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    
    if (paging_get_indexes(virt, &directory_index, &table_index) != 0)
    {
        return; 
    }

    uint32_t entry = directory->directory_entry[directory_index];
    
    uint32_t* table = (uint32_t*)(entry & 0xFFFFF000); 

    if ((entry & PAGING_IS_PRESENT) == 0)
    {
        return; 
    }
    
    table[table_index] = (uint32_t)phys | flags;

    paging_switch(directory->directory_entry); 
}


void paging_switch(uint32_t* directory){
    paging_load_directory(directory);
    current_directory = directory;
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk){
    return chunk->directory_entry;
}