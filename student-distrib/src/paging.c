#include "paging.h"

/* Initializes page table for 0-4mb. Sets video memory to present*/
void init_first_page_table(x86_desc_t first_table_desc){
    unsigned i; 

    for(i = 0; i < NUM_PTE; ++i){
        *(uint32_t *)(first_table_desc.addr + (i << 2)) = (i << 12);
    }
    
    *(uint32_t *)(first_table_desc.addr + (0xB8 << 2)) = (0xB8000 | 0x7);
}

/* Initializes page directory, only first two entries are defined and present for CP1.*/
void init_page_directory(x86_desc_t dir_desc, x86_desc_t first_table_desc){   
                                                      
    *(uint32_t *)(dir_desc.addr) = (first_table_desc.addr & 0xFFFFF000) | 0x007; 
    *(uint32_t *) (dir_desc.addr + 4) = 0x00400087;
}

uint32_t set_pdentry(uint32_t index, pde_desc_t pdentry){
    pde_desc_t* entry_loc = (pde_desc_t *) cr3_desc.addr + (index << 2);
    *entry_loc = pdentry;

    flush_tlb();
    return 0;
}

void flush_tlb(){
    asm volatile ("                 \n\
            mov %%cr3, %%eax        \n\
            mov %%eax, %%cr3        \n\
            "
            :
            :
            : "eax", "memory", "cc"
    );
}
