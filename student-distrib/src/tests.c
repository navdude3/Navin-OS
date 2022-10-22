#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "handle.h"
#include "i8259.h"
#include "mp3fs.h"
#include "vfs.h"

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
	for (i = 0; i < 14; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	for (i = 16; i < 20; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	// Keyboard test
	if ((idt[33].offset_15_00 == NULL) && 
			(idt[33].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
	}

	// RTC
	if ((idt[40].offset_15_00 == NULL) && 
		(idt[40].offset_31_16 == NULL)){
		assertion_failure();
		result = FAIL;
	}


	// Check correct setting of divide zero handler
	uint32_t idt_0_handler = (idt[0].offset_15_00) | (idt[0].offset_31_16 << 16);
	if(idt_0_handler != (uint32_t) divide_error){
		assertion_failure();
		result = FAIL;
	}

	return result;
}

/* div_0_test
 * 
 * Checks if division by 0 causes division by 0 exception
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
// int div_0_test(){
// 	TEST_HEADER;
// 	int result = PASS;
// 	int a = 12/0;
// 	a++;
// 	result = FAIL;
	
// 	return result;
// }


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

/* 
 * rtc_test
 *   DESCRIPTION: checks if "12" alternating interrupt is working. MUST have it enabled in handler
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
*/
int rtc_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	char* video_mem = (char *) 0xB8000;
	// print alternating characters with flag
	int rtc_alt = 0;
	while (rtc_count < 20480){
			if(rtc_count % 1024 == 0 && rtc_alt == 0){  //cond to print 1
			printf("1");
			while(rtc_count % 1024 == 0); // trap to prevent repeats
			
			rtc_alt = 1;
		}
		else if(rtc_count % 1024 == 0 && rtc_alt == 1){ //cond to print 2
			printf("2");
			while(rtc_count % 1024 == 0); // trap to prevent repeats
			rtc_alt = 0;
		}
	}
	char check_char = video_mem[1 << 1];
	if (check_char != '1' && check_char != '2'){ //as per alternating character loop in rtc handler
		assertion_failure();
		result = FAIL;
	}
	return result;
	

}
/* 
 * keyboard test
 *   DESCRIPTION: receives test input from user to check for signal handling. Second line in screen must not be interfered with (e.g., by rtc "12" printing)
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: none
*/
int keyboard_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	printf("Type the following within 10 seconds: asdfg\n");
	char test_char [5] = {'a','s','d','f','g'};
	char* video_mem = (char *) 0xB8000;
	while(rtc_count < 5120);
	int i;
	for(i = 0; i < 5; ++i){
		char check_char = video_mem[(80 + i) << 1];
		if(check_char != test_char[i]){
			assertion_failure();
			result = FAIL;
		}
	}
	return result;
}
// add more tests here

/* Checkpoint 2 tests */

int dir_read_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	uint32_t buf_size = sizeof(dentry_t);
	uint8_t buf[buf_size];
	int32_t cnt;
	uint32_t fd;

	if ((fd = open((uint8_t *)".")) < 0){
		printf("Dir open failed!\n");
		result = FAIL;
		return result;
	}
	while (0 != (cnt = read(fd, buf, buf_size))){
		if (-1 == cnt){
			printf("Dir entry read failed!\n");
			result = FAIL;
			return result;
		}
		dentry_t * d_entry_pt = (dentry_t *) buf;
		char fname [FNAME_LIMIT + 1];
		strncpy(fname, d_entry_pt->file_name, FNAME_LIMIT);
		printf("Filename: %s, Type: %d, Size: %d\n",
				fname, 
				d_entry_pt->file_type,
				init_inode[d_entry_pt->inode_idx].file_size
		);
	}

	d_close(fd);
	return result;
}

int file_read_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	char buf;
	int32_t res, x_cnt;
	uint32_t fd;
	x_cnt = 0;

	char * filename = "frame0.txt";
	if((fd = open((uint8_t *)filename)) < 0){
		printf("File open failed!\n");
		result = FAIL;
		return result;
	}
	while (0 != (res = read(fd, (uint8_t *) &buf, 1))){
		if(-1 == res){
			printf("File read failed!\n");
			result = FAIL;
			return result;
		}		
		if(x_cnt == 79){
			x_cnt = 0;
			putc('\n');
		}
		if(buf == '\n') x_cnt = 0;
		
		++x_cnt;
		putc(buf);
	}
	putc('\n');
	return result;

}
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	/* CP1 Tests */
	// TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("div_0_test", div_0_test());
	// TEST_OUTPUT("invalid_address_test", invalid_address_test());
	// TEST_OUTPUT("deref null", deref_null_test());
	// TEST_OUTPUT("rtc test", rtc_test());
	// TEST_OUTPUT("Keyboard test", keyboard_test());

	/* CP2 Tests*/
	// TEST_OUTPUT("directory read test", dir_read_test());
	TEST_OUTPUT("file read test", file_read_test());
	// launch your tests here
}
