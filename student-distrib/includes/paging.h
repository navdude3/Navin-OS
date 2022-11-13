#ifndef   PAGING_H
#define   PAGING_H

#include "x86_desc.h"
#include "types.h"


#define PAGE_TABLE_SIZE     0x1000 // 4kb
#define PAGE_DIR_SIZE       0x1000 // 4kb
#define PAGE_ENTRY_SIZE     0x400000 // 4MB
#define VIDMAP_TABLE_BASE   (0x8400000 + (6 * PAGE_ENTRY_SIZE)) // after vmem space for user programs

/* Clears TLB */
extern void flush_tlb();
/* Initializes page directory */
extern void init_page_directory(x86_desc_t dir_desc, x86_desc_t first_table_desc);
/* Initializes page table starting at base physical memory */
extern int32_t init__page_table(uint32_t table_base_addr, uint32_t base_phys_addr);

extern void init_paging();
/* Sets the page directory entry and calls flush_tlb */
uint32_t set_pdentry(uint32_t index, pde_desc_t pdentry);

uint32_t set_ptentry(uint32_t table_base_addr, uint32_t idx, pte_desc_t ptentry);
#endif
