#include "types.h"

/* 
 * set_paging_params
 *   DESCRIPTION: sets paging related registers
 *   INPUTS: page directory base register descriptor
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
 *   SIDE EFFECTS: modifies cr0, cr3 and cr4 registers
*/
extern void set_paging_params(uint32_t pdbr);
