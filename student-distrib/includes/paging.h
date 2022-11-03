#define   VIRTUAL_MEM   0x8000000
#define   PAGING_H

#include "x86_desc.h"
#include "types.h"

/* Page directory entry descriptor*/
typedef struct pde_desc {
    union{
        struct{ // Page Entry (4mb)
            uint32_t pde_p : 1;
            uint32_t pde_rw : 1;
            uint32_t pde_us : 1;
            uint32_t pde_pwt : 1;
            uint32_t pde_pcd : 1;
            uint32_t pde_a : 1;
            uint32_t pde_d : 1;
            uint32_t pde_ps : 1;
            uint32_t pde_g : 1;
            uint32_t pde_avail : 3;
            uint32_t pde_pat : 1;
            uint32_t pde_reserved : 9;
            uint32_t pde_pba : 10;
        };
        struct{ // Page Table Entry (4kb)
            uint32_t pde_p : 1;
            uint32_t pde_rw : 1;
            uint32_t pde_us : 1;
            uint32_t pde_pwt : 1;
            uint32_t pde_pcd : 1;
            uint32_t pde_a : 1;
            uint32_t pde_d : 1;
            uint32_t pde_ps : 1;
            uint32_t pde_g : 1;
            uint32_t pde_avail : 3;
            uint32_t pde_ptba : 20;
        };
    };
    
} pde_desc_t;

/* Page Table entry descriptor */
typedef struct pte_desc{
    // Page Entry (4mb)
    uint32_t pte_p : 1;
    uint32_t pte_rw : 1;
    uint32_t pte_us : 1;
    uint32_t pte_pwt : 1;
    uint32_t pte_pcd : 1;
    uint32_t pte_a : 1;
    uint32_t pte_d : 1;
    uint32_t pte_pat : 1;
    uint32_t pte_g : 1;
    uint32_t pte_avail : 3;
    uint32_t pte_pba : 20;
} pte_desc_t;

extern x86_desc_t cr3_desc; // PDBR
extern x86_desc_t first_4_desc; // Page table descriptor for first 4Mb

extern void init_page_directory(dir_desc, first_table_desc);
extern void init_first_page_table(first_table_desc);
uint32_t set_pdentry(uint32_t index, pde_desc_t pdentry);


