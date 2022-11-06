#include "paging.h"

/* 
 * init_first_page_table
 *   DESCRIPTION: Initializes page table for 0-4mb. Sets video memory to present
 *   INPUTS: first_table_desc
 *   RETURN VALUE: None
 *   SIDE EFFECTS: 4MB space allocated 
*/
void init_first_page_table(x86_desc_t first_table_desc){
    unsigned i;

    for(i = 0; i < NUM_PTE; ++i){
        *(uint32_t *)(first_table_desc.addr + (i << 2)) = (i << 12);            /* Left shifting by 2 to find index. Leftshifting by 12 for start of address  */
    }
    
    *(uint32_t *)(first_table_desc.addr + (0xB8 << 2)) = (0xB8000 | 0x7);       /* 0x7 to set video memory as present */
}


/* 
 * init_first_page_table
 *   DESCRIPTION: Initializes page directory, only first two entries are defined and present for CP1.
 *   INPUTS: x86_desc_t dir_desc, x86_desc_t first_table_desc
 *   RETURN VALUE: None
*/
void init_page_directory(x86_desc_t dir_desc, x86_desc_t first_table_desc){   
                                                      
    *(uint32_t *)(dir_desc.addr) = (first_table_desc.addr & 0xFFFFF000) | 0x007; 
    *(uint32_t *) (dir_desc.addr + 4) = 0x00400087;
}


/* 
 * set_pdentry
 *   DESCRIPTION: Sets the page directory entry and flushes TLB
 *   INPUTS: uint32_t index, pde_desc_t pdentry
 *   RETURN VALUE: 0
*/
uint32_t set_pdentry(uint32_t index, pde_desc_t pdentry){
    pde_desc_t* entry_loc = (pde_desc_t *)cr3_desc.addr + index;
    *entry_loc = pdentry;

    flush_tlb();
    return 0;
}

/* 
 * flush_tlb
 *   DESCRIPTION: Flushes TLB when process changes (called by set_pdentry)
 *   INPUTS: None
 *   RETURN VALUE: None
*/
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
