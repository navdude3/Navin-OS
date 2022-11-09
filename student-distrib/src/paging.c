#include "paging.h"
#include "init_helpers.h"

void init_paging(){
    init_page_table(first_4_desc, 0x0); // initialize table for memory 0-4mb
    ((uint32_t *) first_4_desc.addr)[0xB8] = (0xB8000 | 0x3);       /* 0x3 to set video memory as present for supervisor */

    /* Initialize first two entries in page directory */
    // init_page_directory(cr3_desc, first_4_desc);
    ((uint32_t *) cr3_desc.addr)[0] = (first_4_desc.addr & 0xFFFFF000) | 0x3; 
    ((uint32_t *) cr3_desc.addr)[1] = 0x00400083;

    set_paging_params(cr3_desc.addr);
}

/* 
 * init_first_page_table
 *   DESCRIPTION: Initializes page table at the address passed mapping to the base physical address based in.
 *   INPUTS: first_table_desc
 *   RETURN VALUE: None
 *   SIDE EFFECTS: Full page table entry (4096kb) initialized. Any data in the range gets overwritten
*/
int32_t init_page_table(x86_desc_t table_desc, uint32_t phys_base_addr){
    unsigned i;
    uint32_t* table_base_addr; 

    // page table addr alignment check
    if(table_desc.addr & (PAGE_TABLE_SIZE - 1)){
        return -1; // table not aligned to 4kb
    }

    // physical memory aligned to 4MB check
    if(phys_base_addr & PAGE_ENTRY_SIZE - 1){
        return -1; // physical memory not aligned to 4MB (0x400000)
    }
    table_base_addr = (uint32_t *) table_desc.addr;

    for(i = 0; i < NUM_PTE; ++i){
        table_base_addr[i] = phys_base_addr + (i << 12);            /* Left shifting by 2 to find index. Leftshifting by 12 for start of address  */
    }

    return 0;
    
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