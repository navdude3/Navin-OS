#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	clear();

	return result;
}

/* div_0_test
 * 
 * Checks if division by 0 causes division by 0 exception
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
int div_0_test(){
	TEST_HEADER;
	int result = PASS;
	int a = 12/0;
	a++;
	result = FAIL;
	
	return result;
}


/* 
 * invalid_address_test
 *   DESCRIPTION: causes exception to test an address out of range
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
*/
int invalid_address_test(){
	TEST_HEADER;
	int result = PASS;
	uint32_t* ptr = (uint32_t * )0x900000;
	uint32_t a = *ptr;
	a = a+1;
	result = FAIL;
	
	return result;

}

/* 
 * deref_null_test
 *   DESCRIPTION: causes exception to test dereferencing null
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
*/
int deref_null_test(){
	TEST_HEADER;

	int result = PASS;
	uint32_t* ptr = NULL;
	uint32_t a = *ptr;
	a = a+1;
	result = FAIL;
	
	return result;

}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	TEST_OUTPUT("idt_test", idt_test());
	TEST_OUTPUT("div_0_test", div_0_test());
	//TEST_OUTPUT("invalid_address_test", invalid_address_test());
	//TEST_OUTPUT("deref null", deref_null_test());
	// launch your tests here
}
