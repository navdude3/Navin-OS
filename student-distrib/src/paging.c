#include "paging.h"

/* Initializes page table for 0-4mb. Sets video memory to present*/
init_first_page_table(first_table_desc){
    unsigned i; 

    for(i = 0; i < NUM_PTE; ++i){
        *(uint32_t *)(first_table_desc.addr + (i << 2)) = (i<<12);
    }
    
    *(uint32_t *)(first_table_desc.addr + (0xB8 << 2)) = (0xB8000 |0x7);
}

/* Initializes page directory, only first two entries are defined and present for CP1.*/
init_page_directory(dir_desc, first_table_desc){   
                                                      
    *(uint32_t *)(dir_desc.addr) = (first_table_desc.addr & 0xFFFFF000) | 0x007; 
    *(uint32_t *) (dir_desc.addr + 4) = 0x00400087;
}

