#include <stdio.h>
#include <string.h>

#include "code_parser.h"
#include "error_handler.h"
#include "vm_assembly.h"



static FILE *asm_file;
//current line in asm file that is being written
static int current_asm_line;
//buffer to hold the decoded instructions received from code_parser
//instruction_buffer[0] holds which type of instruction is used, forexample: push,pop,arithmetic,call,function etc.
//instruction_buffer[1] holds the first argument, forexample: constant,static,local,argument,add,sub,lt etc.
//instruction_buffer[2] holds the second argument, forexample: numbers like 2, 34, 865 or variables like x,y,a_d,square etc.
static int instruction_buffer[3];
//buffer to hold symbols (variables/labels) received from code_parser
static char symbol_buffer[MAX_SYMBOL_LENGTH];

//flags to keep track if eq, gt or lt were already written to asm file
//cuz some parts of the eq, gt and lt do not need to be written multiple times
static bool already_eq = false;
static bool already_gt = false;
static bool already_lt = false;



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

//writes a comparison operation (eq, gt, lt) to .asm file, keeps track of proper label names for jump instructions in comparison operation
void write_comparison_operation(int operation) {
	static int n_eq = 0; //how many eq operations have been written, this will be used as a suffix to the labels for jump instructions in the eq operation to allow proper jumps
	static int n_gt = 0;
	static int n_lt = 0;

	static char comp_buffer[128] = {0};


	/*
	template for comparison operation with eq as example:
	comparison_start
	eq_true with eq_jump
	eq_false with always_jump
	(eq_true)
	comparison_true
	eq_end with always_jump
	(eq_false)
	comparison_false
	(eq_end)
	comparison_end
	*/

	//comparison_start
	fputs(comparison_start, asm_file);


	//write:
	//eq_true with eq_jump
	//eq_false with always_jump
	//(eq_true)
	switch (operation)
	{
	case A_EQ:
		//eq_true with eq_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[0], n_eq);
		fputs(comp_buffer, asm_file);
		fputs("D;JEQ\n", asm_file);

		//eq_false with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[1], n_eq);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(eq_true)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[0], n_eq);
		fputs(comp_buffer, asm_file);
		break;

	case A_GT:
		//gt_true with gt_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[3], n_gt);
		fputs(comp_buffer, asm_file);
		fputs("D;JGT\n", asm_file);

		//gt_false with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[4], n_gt);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(gt_true)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[3], n_gt);
		fputs(comp_buffer, asm_file);
		break;

	case A_LT:

		//lt_true with lt_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[6], n_lt);
		fputs(comp_buffer, asm_file);
		fputs("D;JLT\n", asm_file);

		//lt_false with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[7], n_lt);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(lt_true)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[6], n_lt);
		fputs(comp_buffer, asm_file);
		break;
	
	default:
		handle_error(OTHER_ERROR, true, "Unknown comparison operation encountered! Called by write_comparison_operation()");
		break;
	}
	
	//comparison_true
	fputs(comparison_true, asm_file);

	//write:
	//eq_end with always_jump
	//(eq_false)
	switch (operation)
	{
	case A_EQ:
		
		//eq_end with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[2], n_eq);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(eq_false)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[1], n_eq);
		fputs(comp_buffer, asm_file);
		break;

	case A_GT:
		
		//gt_end with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[5], n_gt);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(gt_false)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[4], n_gt);
		fputs(comp_buffer, asm_file);
		break;

	case A_LT:

		//lt_end with always_jump
		sprintf(comp_buffer, "@%s.%i\n", comparison_labels[8], n_lt);
		fputs(comp_buffer, asm_file);
		fputs("D;JMP\n", asm_file);

		//(lt_false)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[7], n_lt);
		fputs(comp_buffer, asm_file);
		break;
	
	default:
		handle_error(OTHER_ERROR, true, "Unknown comparison operation encountered! Called by write_comparison_operation()");
		break;
	}

	//comparison_false
	fputs(comparison_false, asm_file);

	//write:
	//(eq_end)
	switch (operation)
	{
	case A_EQ:
	
		//(eq_end)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[2], n_eq);
		fputs(comp_buffer, asm_file);
		break;

	case A_GT:

		//(gt_end)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[5], n_gt);
		fputs(comp_buffer, asm_file);
		break;

	case A_LT:

		//(lt_end)
		sprintf(comp_buffer, "(%s.%i)\n", comparison_labels[8], n_lt);
		fputs(comp_buffer, asm_file);
		break;
	
	default:
		handle_error(OTHER_ERROR, true, "Unknown comparison operation encountered! Called by write_comparison_operation()");
		break;
	}

	//comparison_end
	fputs(comparison_end, asm_file);

	//incrementing counters
	switch (operation)
	{
	case A_EQ:
		printf("Wrote \"eq.%i\" to assembly file!\n", n_eq);
		n_eq++;
		break;

	case A_GT:
		printf("Wrote \"gt.%i\" to assembly file!\n", n_gt);
		n_gt++;
		break;

	case A_LT:
		printf("Wrote \"lt.%i\" to assembly file!\n", n_lt);
		n_lt++;
		break;
	
	default:
		break;
	}


}

//write an arithmetic operation to .asm file
void write_arithmetic(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size) {
	//deciding which type of arithmetic is needed
	switch (decoded_instruction_buffer[1])
	{
	case A_ADD:
		fputs(add, asm_file);
		printf("Wrote an \"add\" to assembly file!\n");
		break;

	case A_SUB:
		fputs(sub, asm_file);
		printf("Wrote a \"sub\" to assembly file!\n");
		break;

	case A_NEG:
		fputs(neg, asm_file);
		printf("Wrote a \"neg\" to assembly file!\n");
		break;

	case A_EQ:
		write_comparison_operation(decoded_instruction_buffer[1]);
		break;

	case A_GT:
		write_comparison_operation(decoded_instruction_buffer[1]);
		break;

	case A_LT:
		write_comparison_operation(decoded_instruction_buffer[1]);
		break;

	case A_AND:
		fputs(and, asm_file);
		printf("Wrote an \"and\" to assembly file!\n");
		break;

	case A_OR:
		fputs(or, asm_file);
		printf("Wrote a \"or\" to assembly file!\n");
		break;

	case A_NOT:
		fputs(not, asm_file);
		printf("Wrote a \"not\" to assembly file!\n");
		break;
	
	default:
		printf("Wrote a \"UNKNOWN\" to assembly file!\n");
		handle_error(OTHER_ERROR, true, "Unknown arithmetic instruction encountered while tryin to write it to assembly file!");
		break;
	}
}


//write @Xxx with constant = Xxx to .asm file
void write_constant(int constant) { 
	char line_buffer[128] = {0};
	sprintf(line_buffer, "@%i\n", constant);
	fputs(line_buffer, asm_file);
}

//write push operation to .asm file
void write_push(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size) {
	//deciding which type of memory push is needed
	switch (decoded_instruction_buffer[1])
	{
	case M_CONSTANT:
		write_constant(decoded_instruction_buffer[2]);
		fputs(push_constant,asm_file);
		printf("Wrote a \"push constant %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case A_SUB:
		fputs(sub, asm_file);
		printf("Wrote a \"sub\" to assembly file!\n");
		break;
	
	default:
		break;
	}
}



//translate the received instructions into assembly and write that into the opened file
void write_instruction(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size) {
	//checking size of buffers
	if(decoded_instruction_buffer_size < 3) {
		handle_error(OTHER_ERROR, true, "Buffer to hold decoded instruction is not big enough! (size must be >=3)");
	}
	else if(symbol_buffer_size < MAX_SYMBOL_LENGTH) {
		handle_error(OTHER_ERROR, true, "Buffer to hold symbol_name is nog big enough! must be >=MAX_SYMBOL_LENGTH");
	}


	//determining type of instruction
	switch (decoded_instruction_buffer[0])
	{
	case C_ARTIHMETIC:
		write_arithmetic(decoded_instruction_buffer, decoded_instruction_buffer_size, symbol_buffer, symbol_buffer_size);
		break;

	case C_PUSH:
		//write push instructions
		write_push(decoded_instruction_buffer, decoded_instruction_buffer_size, symbol_buffer, symbol_buffer_size);
		break;

	case C_POP:
		//write pop instructions
		break;
	
	default:
		break;
	}
}



//closes the .asm file
void close_asm_file() {
	int error_status = fclose(asm_file);
	if(error_status == EOF) {
		handle_error(OTHER_ERROR, true, "Cannot close .asm file!");
	}
}