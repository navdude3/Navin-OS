#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "handle.h"
#include "i8259.h"
#include "mp3fs.h"
#include "vfs.h"
#include "terminal.h"
#include "loader.h"

#define PASS 1
#define FAIL 0
#define MAX 1024
#define MIN 2
#define MAX_RTC 20480
#define ROWEND 79
#define PRINT 5
#define BYTES 4
#define NINEBYTES 36
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
	while (rtc_count < MAX_RTC){
			if(rtc_count % MAX == 0 && rtc_alt == 0){  //cond to print 1
			printf("1");
			while(rtc_count % MAX == 0); // trap to prevent repeats
			
			rtc_alt = 1;
		}
		else if(rtc_count % MAX == 0 && rtc_alt == 1){ //cond to print 2
			printf("2");
			while(rtc_count % MAX == 0); // trap to prevent repeats
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
	char test_char [PRINT] = {'a','s','d','f','g'};
	char* video_mem = (char *) 0xB8000;
	while(rtc_count < MAX * PRINT);
	int i;
	for(i = 0; i < PRINT; ++i){
		char check_char = video_mem[(ROWEND + 1 + i) << 1];
		if(check_char != test_char[i]){
			assertion_failure();
			result = FAIL;
		}
	}
	return result;
}

/* Checkpoint 2 tests */
/* 
 * directory read test
 *   DESCRIPTION: Opens the top level directory "." and prints all entries with name, type and size.
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL if any read/open/close errors occur
 *   RETURN VALUE: none
*/

int dir_read_test(){

	TEST_HEADER;

	int result = PASS;
	clear();
	uint32_t buf_size = sizeof(dentry_t);
	uint8_t buf[buf_size]; // declare buffer big enough to fit dentry_t
	int32_t cnt;
	uint32_t fd;

	if ((fd = open((uint8_t *)".")) < 0){
		printf("Dir open failed!\n");
		result = FAIL;
		return result;
	}
	/* Read directory entries one by one until EOF */
	while (0 != (cnt = read(fd, buf, buf_size))){
		if (-1 == cnt){
			printf("Dir entry read failed!\n");
			result = FAIL;
			return result;
		}
		dentry_t * d_entry_pt = (dentry_t *) buf;
		char fname [FNAME_LIMIT + 1]; // Copy file_name from dentry into string to deal with long names 
		strncpy(fname, d_entry_pt->file_name, FNAME_LIMIT);
		printf("Filename: %s, Type: %d, Size: %d\n",
				fname, 
				d_entry_pt->file_type,
				init_inode[d_entry_pt->inode_idx].file_size
		);
	}

	close(fd);
	return result;
}

/* 
 * regular file read test
 *   DESCRIPTION: Opens the "frame0.txt" and prints all contents. 
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL if any read/open/close errors occur
 *   RETURN VALUE: none
*/
int file_read_test(const char* filename){
	TEST_HEADER;

	int result = PASS;
	clear();
	char buf;
	int32_t res, x_cnt;
	uint32_t fd;
	x_cnt = 0;

	// char * filename = "frame0.txt";
	if((fd = open((uint8_t *)filename)) < 0){
		printf("File open failed!\n");
		result = FAIL;
		return result;
	}

	/* Read file byte by byte, print character to screen */
	while (0 != (res = read(fd, (uint8_t *) &buf, 1))){
		if(-1 == res){
			printf("File read failed!\n");
			result = FAIL;
			return result;
		}
		/* if end of row, go to next line (for non text files)*/		
		if(x_cnt == ROWEND){
			x_cnt = 0;
			putc('\n');
		}
		if(buf == '\n') x_cnt = 0;
		
		++x_cnt;
		putc(buf);
	}
	putc('\n');
	close(fd);
	return result;
}
/* 
 * executable file read test
 *   DESCRIPTION: Opens the "ls" and checks if it matches ELF specs (start identifier and ending magic string). 
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL if any read/open/close errors occur OR if file does not match ELF spec per MP3.2 documentation
 *   RETURN VALUE: none
*/
int file_executable_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	int middle_padding_amt = 5308; 						// Size of ls is 5349, 5349 - 4 (identity header) - 36 (end magic str) = 5308
	char start_ident_buf [5]; 							// magic characters + null terminator (5 characters)
	char* start_ident_str = "\x7f""ELF";
	char m_buf;											// middle padding read buf
	char end_ident_buf[37]; 							// end magic characters (36) + null terminator
	char* end_ident_str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int32_t res, x_cnt;
	uint32_t fd;
	x_cnt = 0;

	char * filename = "ls";
	if((fd = open((uint8_t *)filename)) < 0){
		printf("File open failed!\n");
		result = FAIL;
		return result;
	}
	

	/* Read first 4 bytes for 0x7f, E, L, F*/
	res = read(fd, (uint8_t *) start_ident_buf, BYTES);
	start_ident_buf[BYTES] = '\0';
	if(-1 == res){
		printf("File read failed!\n");
		result = FAIL;
		return result;
	}
	if(strncmp((int8_t*)start_ident_buf, (int8_t*)start_ident_str, BYTES) != 0){
		printf("Starting magic characters not read! Read this instead: %s", start_ident_buf);
		result = FAIL;
		return result;
	}

	/* Skip till last 31 chars*/
	unsigned i;
	for(i = 0; i < middle_padding_amt; ++i){
		res = read(fd,(uint8_t *) &m_buf, 1);
		if(-1 == res){
			printf("Middle padding read fail!\n");
			result = FAIL;
			return result;
		}
	}

	/* Read ending magic string */
	res = read(fd, (uint8_t *) end_ident_buf, NINEBYTES);
	end_ident_buf[NINEBYTES] = '\0';
	if(-1 == res){
		printf("File read failed!\n");
		result = FAIL;
		return result;
	}
	if(strncmp((int8_t*)end_ident_buf, (int8_t*)end_ident_str, NINEBYTES) != 0){
		printf("Ending magic characters do not match! Read this instead: %s", end_ident_buf);
		result = FAIL;
		return result;
	}
	


	putc('\n');
	close(fd);
	return result;
}


/* 
 * file_bad_filename_test
 *   DESCRIPTION: Sees if file name is a real file name 
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL if any read/open/close errors occur OR if file does not match ELF spec per MP3.2 documentation
 *   RETURN VALUE: none
*/
int file_bad_filename_test(){
	TEST_HEADER;

	int result = PASS;
	clear();
	

	char * filename = "nonexistentfile.txt";
	if( open((uint8_t *)filename) >= 0){
		printf("Nonexistent file opened!\n");
		result = FAIL;
		return result;
	}
	return result;
}



/* 
 * terminal_RW_test_nobug
 *   DESCRIPTION: send a buffer of 128 and write from keyboard to it
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL buffer could not be wrote back to
 *   RETURN VALUE: none
*/
int terminal_RW_test_nobug(){
	TEST_HEADER;
	int result = PASS;
	clear();

	char user_buffer[128];			//size of buffer getting passed in
	printf("Type your name\n");
	read(1, (uint8_t *) user_buffer, 128);		//read input from user
	printf("Hello ");
	write(1,(uint8_t *) user_buffer, 128);		//write it to screen

	return result;
}

/* 
 * terminal_RW_test_overflow
 *   DESCRIPTION: take a buffer of over 128 and only push first 128 charachtres
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL if buffer could not be read back
 *   RETURN VALUE: none
*/
int terminal_RW_test_overflow(){
	TEST_HEADER;
	int result = PASS;
	clear();

	char user_buffer[128];					//user buffer to pass in
	printf("Type over 128 charachters\n");
	read(1,(uint8_t *) user_buffer, 200);		//overflow byte size
	write(1,(uint8_t *) user_buffer, 200);

	return result;
}

/* 
 * terminal_open_and_close
 *   DESCRIPTION: open and close the terminal
 *   INPUTS: none
 *   OUTPUTS: PASS if no errors, FAIL terminal fails to open
 *   RETURN VALUE: none
*/
int terminal_open_and_close(){
	TEST_HEADER;
	int result = PASS;
	clear();

	
	if( -1 != close(1)){		//if close fails then fail the test
		result = FAIL;
		return result;
	}

	return result;
}


/* 
 * rtc_read_write_test
 *   DESCRIPTION: tests open, read and write functions of RTC device
 *   INPUTS: none
 *   OUTPUTS: check variable printed i times for each frequency i
 *   RETURN VALUE: PASS/FAIL
*/
int rtc_read_write_test() {
    TEST_HEADER;
	int result = PASS;
	clear();
    int i, j;
	int check = 0;

    check += rtc_open(NULL);							// call rtc_open
    
	for(i = MIN; i <= MAX; i = i * MIN) {
        check += rtc_write(NULL, &i, sizeof(int)); 		// call rtc_write with each frequency
		printf("%d", i);
		for(j = 0; j < i/MIN; j++) {
            check += rtc_read(NULL, NULL, NULL); 		
			printf("%d", check);						// call rtc_read to read the frequency and print check
        }

        printf("]\n");
    }
	
	check += rtc_close(0);								// call rtc_close
	
	if(check == 0) return result; 						// make sure check is 0 after all RTC calls are done
	else{
		result = FAIL;
		return result;
	}
}



/* Checkpoint 3 tests */

int execute_test(){
    TEST_HEADER;
	int result = PASS;

	char * command = "hello";
	sys_execute((uint8_t*) command);
	return result;
}





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
	// TEST_OUTPUT("file read test", file_read_test("frame0.txt"));
	// TEST_OUTPUT("file long name read test", file_read_test("verylargetextwithverylongname.txt"));
	// TEST_OUTPUT("Nonexistent file read test", file_bad_filename_test());
	// TEST_OUTPUT("Executable file read test", file_executable_test());
	// TEST_OUTPUT("Terminal RW Test", terminal_RW_test_nobug());
	// TEST_OUTPUT("Terminal RW Overflow Test", terminal_RW_test_overflow());
	// TEST_OUTPUT("Terminal open and close", terminal_open_and_close());
	// TEST_OUTPUT("RTC read write test", rtc_read_write_test());
	//TEST_OUTPUT("hello syscall", syscall_hello());
	TEST_OUTPUT("execute", execute_test());
	// launch your tests here
}
