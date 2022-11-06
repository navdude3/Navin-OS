#define   VIRTUAL_MEM   0x8000000
#define   PAGING_H

#include "x86_desc.h"
#include "types.h"

/* Clears TLB */
extern void flush_tlb();
/* Initializes page directory */
extern void init_page_directory(x86_desc_t dir_desc, x86_desc_t first_table_desc);
/* Initializes page table for physical memory 0-4MB */
extern void init_first_page_table(x86_desc_t first_table_desc);
/* Sets the page directory entry and calls flush_tlb */
uint32_t set_pdentry(uint32_t index, pde_desc_t pdentry);


