#include <stdio.h>
#include <string.h>

#include "code_parser.h"
#include "error_handler.h"



static FILE *asm_file;
//current line in asm file that is being written
static int current_line;
//buffer to hold the decoded instructions received from code_parser
//instruction_buffer[0] holds which type of instruction is used, forexample: push,pop,arithmetic,call,function etc.
//instruction_buffer[1] holds the first argument, forexample: constant,static,local,argument,add,sub,lt etc.
//instruction_buffer[2] holds the second argument, forexample: numbers like 2, 34, 865 or variables like x,y,a_d,square etc.
static int instruction_buffer[3];
//buffer to hold symbols (variables/labels) received from code_parser
static char symbol_buffer[MAX_SYMBOL_LENGTH];



//uses get_file_name() to get the name for the .asm file
void open_asm_file(char *argv[], bool debug) {
	//open asm file
	char file_name[128] = {0};
	get_file_name(argv, file_name, sizeof(file_name), debug);

	strcat(file_name, ".asm");

	asm_file = fopen(file_name, "w");
	if(asm_file == NULL) {
		handle_error(OTHER_ERROR, true, "Cannot open .asm file!");
	}
}

//translate the received instructions into assembly and write that into the opened file
void write_instruction(int *int_buffer, int int_buffer_size,  char *name_buffer, int name_buffer_size) {

}

//closes the .asm file
void close_asm_file() {
	int error_status = fclose(asm_file);
	if(error_status == EOF) {
		handle_error(OTHER_ERROR, true, "Cannot close .asm file!");
	}
}