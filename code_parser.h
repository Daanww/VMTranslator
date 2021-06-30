#ifndef CODE_PARSER
#define CODE_PARSER

#include <stdbool.h>

#define MAX_LINE_LENGTH 256
#define MAX_SYMBOL_LENGTH 128

//enums for decoded instruction parts
enum decoded_instruction_parts{
	//commands
	C_ARTIHMETIC = 1,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL,

	//name command
	N_NAME,

	//arithmetic commands
	A_ADD,
	A_SUB,
	A_NEG,
	A_EQ,
	A_GT,
	A_LT,
	A_AND,
	A_OR,
	A_NOT,

	//memory segments
	M_ARGUMENT,
	M_LOCAL,
	M_STATIC,
	M_CONSTANT,
	M_THIS,
	M_THAT,
	M_POINTER,
	M_TEMP,
};



void index_files(int argc, char *argv[], bool debug);
void get_file_name(char *argv[], char *buffer, int buffer_size, bool debug);
int get_total_num_files();
int open_next_vm_file();
void get_current_file_name(char* buffer, int buffer_size);
int get_current_line();
int read_line();
void format_line();
void decode_line(int *int_buffer, int int_buffer_size, char *name_buffer, int name_buffer_size);
void close_current_vm_file();

#endif