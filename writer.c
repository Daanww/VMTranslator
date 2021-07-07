#include <stdio.h>
#include <string.h>

#include "code_parser.h"
#include "error_handler.h"
#include "vm_assembly.h"


//current asm file being written to
static FILE *asm_file;
//current line in asm file that is being written
static int current_asm_line;
//current function that is being written, used for labels and return addresses
static char current_function[MAX_SYMBOL_LENGTH];


//buffer to hold the decoded instructions received from code_parser
//instruction_buffer[0] holds which type of instruction is used, forexample: push,pop,arithmetic,call,function etc.
//instruction_buffer[1] holds the first argument, forexample: constant,static,local,argument,add,sub,lt etc.
//instruction_buffer[2] holds the second argument, forexample: numbers like 2, 34, 865 or variables like x,y,a_d,square etc.
static int instruction_buffer[3];
//buffer to hold symbols (variables/labels) received from code_parser
static char symbol_buffer[MAX_SYMBOL_LENGTH];
//buffer to hold the current function name being processed
static char function_buffer[MAX_SYMBOL_LENGTH];



//uses get_file_name() to get the name for the .asm file
void open_asm_file(char *argv[]) {
	//open asm file
	char file_name[128] = {0};
	get_file_or_dir_name(argv, file_name, sizeof(file_name));

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
	fputs(push_constant,asm_file);
}

void write_static(int *decoded_instruction_buffer) {
	//constructing variable name from file_name and instruction arguments
	char variable_name[MAX_SYMBOL_LENGTH] = {0};
	variable_name[0] = '@';
	char file_name[MAX_SYMBOL_LENGTH] = {0};
	get_current_file_name(file_name, sizeof(file_name));
	strcat(variable_name, file_name); //append file name to @
	sprintf(file_name, ".%i\n", decoded_instruction_buffer[2]);
	strcat(variable_name, file_name); //append .number\n to @file_name

	//checking for push/pop
	if(decoded_instruction_buffer[0] == C_PUSH) {
		fputs(variable_name, asm_file);
		fputs(push_static, asm_file);
	}
	else if(decoded_instruction_buffer[0] == C_POP) {
		fputs(pop_static_1, asm_file);
		fputs(variable_name, asm_file);
		fputs(pop_static_2, asm_file);
	}

}

void write_pointer(int *decoded_instruction_buffer) {
	char pointer_buffer[MAX_SYMBOL_LENGTH] = {0};
	sprintf(pointer_buffer, "@%i\n", (3 + decoded_instruction_buffer[2]));
	if(decoded_instruction_buffer[0] == C_PUSH) {
		fputs(pointer_buffer, asm_file);
		fputs(push_pointer, asm_file);
	}
	else if(decoded_instruction_buffer[0] == C_POP) {
		fputs(pop_pointer_1, asm_file);
		fputs(pointer_buffer, asm_file);
		fputs(pop_pointer_2, asm_file);
	}
}

void write_this_that(int *decoded_instruction_buffer) {
	char this_that_buffer[MAX_SYMBOL_LENGTH] = {0};
	sprintf(this_that_buffer, "@%i\n", decoded_instruction_buffer[2]);
	if(decoded_instruction_buffer[0] == C_PUSH) {
		if(decoded_instruction_buffer[1] == M_THIS) {
			fputs(push_this_1, asm_file);
			fputs(this_that_buffer, asm_file);
		}
		else if(decoded_instruction_buffer[1] == M_THAT) {
			fputs(push_that_1, asm_file);
			fputs(this_that_buffer, asm_file);
		}
		fputs(push_this_that_2, asm_file);
	}
	else if(decoded_instruction_buffer[0] == C_POP) {
		if(decoded_instruction_buffer[1] == M_THIS) {
			fputs(pop_this_1, asm_file);
			fputs(this_that_buffer, asm_file);
		}
		else if(decoded_instruction_buffer[1] == M_THAT) {
			fputs(pop_that_1, asm_file);
			fputs(this_that_buffer, asm_file);
		}
		fputs(pop_this_that_2, asm_file);
	}
}

void write_local_argument(int *decoded_instruction_buffer) {
	char local_argument_buffer[MAX_SYMBOL_LENGTH] = {0};
	sprintf(local_argument_buffer, "@%i\n", decoded_instruction_buffer[2]);
	if(decoded_instruction_buffer[0] == C_PUSH) {
		if(decoded_instruction_buffer[1] == M_LOCAL) {
			fputs(push_local_1, asm_file);
			fputs(local_argument_buffer, asm_file);
		}
		else if(decoded_instruction_buffer[1] == M_ARGUMENT) {
			fputs(push_argument_1, asm_file);
			fputs(local_argument_buffer, asm_file);
		}
		fputs(push_local_argument_2, asm_file);
	}
	else if(decoded_instruction_buffer[0] == C_POP) {
		if(decoded_instruction_buffer[1] == M_LOCAL) {
			fputs(pop_local_1, asm_file);
			fputs(local_argument_buffer, asm_file);
		}
		else if(decoded_instruction_buffer[1] == M_ARGUMENT) {
			fputs(pop_argument_1, asm_file);
			fputs(local_argument_buffer, asm_file);
		}
		fputs(pop_local_argument_2, asm_file);
	}
}

void write_temp(int *decoded_instruction_buffer) {
	//write_temp is essentially the same as write_pointer, just with a different base address
	char temp_buffer[MAX_SYMBOL_LENGTH] = {0};
	sprintf(temp_buffer, "@%i\n", (5 + decoded_instruction_buffer[2]));
	if(decoded_instruction_buffer[0] == C_PUSH) {
		fputs(temp_buffer, asm_file);
		fputs(push_temp, asm_file);
	}
	else if(decoded_instruction_buffer[0] == C_POP) {
		fputs(pop_temp_1, asm_file);
		fputs(temp_buffer, asm_file);
		fputs(pop_temp_2, asm_file);
	}
}

//write push operation to .asm file
void write_push(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size) {
	//deciding which type of memory push is needed
	switch (decoded_instruction_buffer[1])
	{

	case M_LOCAL:
		write_local_argument(decoded_instruction_buffer);
		printf("Wrote a \"push local %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
		
	case M_ARGUMENT:
		write_local_argument(decoded_instruction_buffer);
		printf("Wrote a \"push argument %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_STATIC:
		write_static(decoded_instruction_buffer);
		printf("Wrote a \"push static %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_CONSTANT:
		write_constant(decoded_instruction_buffer[2]);		
		printf("Wrote a \"push constant %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_THIS:
		write_this_that(decoded_instruction_buffer);
		printf("Wrote a \"push this %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
		
	case M_THAT:
		write_this_that(decoded_instruction_buffer);
		printf("Wrote a \"push that %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_POINTER:
		write_pointer(decoded_instruction_buffer);
		printf("Wrote a \"push pointer %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_TEMP:
		write_temp(decoded_instruction_buffer);
		printf("Wrote a \"push temp %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	
	default:
		break;
	}
}

void write_pop(int *decoded_instruction_buffer, int decoded_instruction_buffer_size,  char *symbol_buffer, int symbol_buffer_size) {
	
	//deciding which type of memory pop is needed
	switch (decoded_instruction_buffer[1])
	{

	case M_LOCAL:
		write_local_argument(decoded_instruction_buffer);
		printf("Wrote a \"pop local %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
		
	case M_ARGUMENT:
		write_local_argument(decoded_instruction_buffer);
		printf("Wrote a \"pop argument %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_STATIC:
		write_static(decoded_instruction_buffer);
		printf("Wrote a \"pop static %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	case M_THIS:
		write_this_that(decoded_instruction_buffer);
		printf("Wrote a \"pop this %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
		
	case M_THAT:
		write_this_that(decoded_instruction_buffer);
		printf("Wrote a \"pop that %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
	

	case M_POINTER:
		write_pointer(decoded_instruction_buffer);
		printf("Wrote a \"pop pointer %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;
	
	case M_TEMP:
		write_temp(decoded_instruction_buffer);
		printf("Wrote a \"pop temp %i\" to assembly file!\n", decoded_instruction_buffer[2]);
		break;

	default:
		break;
	}
}

//writing a label to asm file, label symbol_name is translated to (function_name$symbol_name) to allow jumping there
void write_label(char *symbol_buffer) {
	char asm_label_buffer[MAX_SYMBOL_LENGTH] = {0};
	if(function_buffer[0] == 0) { //if current function is undefined/no function has been found yet
		strcpy(asm_label_buffer, "(UNDEFINED_FUNCTION$");
	}
	else {
		asm_label_buffer[0] = '(';
		strcat(asm_label_buffer, function_buffer);
		strcat(asm_label_buffer, "$");
	}
	strcat(asm_label_buffer, symbol_buffer);
	strcat(asm_label_buffer, ")\n");
	fputs(asm_label_buffer, asm_file);
}

//writes a goto to label to asm file, this will always jump
void write_goto(char *symbol_buffer) {
	char asm_goto_buffer[MAX_SYMBOL_LENGTH] = {0};
	if(function_buffer[0] == 0) { //if current function is undefined/no function has been found yet
		strcpy(asm_goto_buffer, "@UNDEFINED_FUNCTION$");
	}
	else {
		asm_goto_buffer[0] = '@';
		strcat(asm_goto_buffer, function_buffer);
		strcat(asm_goto_buffer, "$");
	}
	strcat(asm_goto_buffer, symbol_buffer);
	strcat(asm_goto_buffer, "\n");
	fputs(asm_goto_buffer, asm_file);
	fputs("D;JMP\n", asm_file);
}


//writes a if_goto to label to asm file, this is a conditional jump
//this will first pop the topmost entry in the stack, if that is !0 then it will jump
//if the popped entry is 0 then it will continue execution at the next instruction
void write_if_goto(char *symbol_buffer) {
	//popping top most stack entry and storing it in D
	char pop_stack[] = "@0\nM=M-1\nA=M\nD=M\n";
	fputs(pop_stack, asm_file);

	char asm_if_goto_buffer[MAX_SYMBOL_LENGTH] = {0};
	if(function_buffer[0] == 0) { //if current function is undefined/no function has been found yet
		strcpy(asm_if_goto_buffer, "@UNDEFINED_FUNCTION$");
	}
	else {
		asm_if_goto_buffer[0] = '@';
		strcat(asm_if_goto_buffer, function_buffer);
		strcat(asm_if_goto_buffer, "$");
	}
	strcat(asm_if_goto_buffer, symbol_buffer);
	strcat(asm_if_goto_buffer, "\n");
	fputs(asm_if_goto_buffer, asm_file);
	fputs("D;JNE\n", asm_file);
}

//writes function declaration to asm file, also stores current function name in function_buffer for use with call and return
void write_function(int *decoded_instruction_buffer, char *symbol_buffer) {
	strcpy(function_buffer, symbol_buffer); //copy function name to function_buffer
	//write (function_name) to asm
	char temp_buffer[MAX_LINE_LENGTH] = {0};
	sprintf(temp_buffer, "(%s)\n", function_buffer);
	fputs(temp_buffer, asm_file);
	
	//writing "push constant 0" for all local variables needed by function
	for(int i = 0; i < decoded_instruction_buffer[2]; i++) {
		write_constant(0);
	}
}

//writes return to asm file
void write_return() {
	/* pseudo code for return:
	FRAME = LCL ; setup local variable
	RET = *(FRAME-5) ; get return-address
	*ARG = pop()
	SP = ARG+1
	THAT = *(FRAME-1)
	THIS = *(FRAME-2)
	ARG = *(FRAME-3)
	LCL = *(FRAME-4)
	goto RET
	*/

	//FRAME = LCL
	//FRAME is a temp variable, here R13 (general purpose register) is used
	fputs(FRAME_LCL, asm_file);

	//RET = *(FRAME-5)
	//RET is another temp variable used to hold return-address, R14 is used
	fputs(RET_FRAME_5, asm_file);

	//*ARG = pop()
	//pop a value of the stack and store that in the location pointed at by ARG
	fputs(ARG_EQ_POP, asm_file);

	//SP = ARG+1
	fputs(SP_ARG_1, asm_file);

	//THAT = *(FRAME-1)
	//THIS = *(FRAME-2)
	//ARG = *(FRAME-3)
	//LCL = *(FRAME-4)
	fputs(restore_memory_segments, asm_file);

	//goto RET
	fputs(GOTO_RET, asm_file);
}

//writes function call to asm file
void write_call(int *decoded_instruction_buffer, char *symbol_buffer) {
	/* pseudo code for function call:
	push return-address ; label declared below
	push LCL ; save state of calling function
	push ARG
	push THIS
	push THAT
	ARG = SP-n-5 ; n=number of arguments for function == decoded_instruction_buffer[2]
	LCL = SP
	goto f ; transfer control
	(return-address)
	*/

	char return_address[MAX_SYMBOL_LENGTH] = {0}; //buffer for generating unique return address for this function call
	static int n_returns = 0; //counter for amount of function calls that have been made, used in generation of above
	//generating return_address
	sprintf(return_address, "%s.%i", symbol_buffer, n_returns);
	//incrementing counter
	n_returns++;

	//push return-address
	char temp_buffer[MAX_LINE_LENGTH] = {0};
	sprintf(temp_buffer, "@%s\n", return_address);
	fputs(temp_buffer, asm_file);
	fputs(push_return_address, asm_file);

	//push LCL
	fputs("@LCL\n", asm_file);
	fputs(push_static, asm_file);

	//push ARG
	fputs("@ARG\n", asm_file);
	fputs(push_static, asm_file);

	//push THIS
	fputs("@THIS\n", asm_file);
	fputs(push_static, asm_file);

	//push THAT
	fputs("@THAT\n", asm_file);
	fputs(push_static, asm_file);

	//ARG = SP-n-5
	sprintf(temp_buffer, "@%i\n", decoded_instruction_buffer[2]); //setting up @n
	fputs(ARG_SP_n_5_1, asm_file);
	fputs(temp_buffer, asm_file);
	fputs(ARG_SP_n_5_2, asm_file);

	//LCL = SP
	fputs(LCL_SP, asm_file);

	//goto f
	sprintf(temp_buffer, "@%s\n", symbol_buffer);
	fputs(temp_buffer, asm_file);
	fputs("D;JMP\n", asm_file);

	//(return-address)
	sprintf(temp_buffer, "(%s)\n", return_address);
	fputs(temp_buffer, asm_file);

}

//writes init/bootstrap code
void write_init_code() {
	fputs(init_code, asm_file);
	int decoded_instruction_buffer[3] = {C_CALL, N_NAME, 0};
	char symbol_buffer[MAX_SYMBOL_LENGTH] = "Sys.init";
	write_call(decoded_instruction_buffer, symbol_buffer);
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

	case 0:
		//empty line, skip
		break;
		
	case C_ARTIHMETIC:
		write_arithmetic(decoded_instruction_buffer, decoded_instruction_buffer_size, symbol_buffer, symbol_buffer_size);
		break;

	case C_PUSH:
		//write push instructions
		write_push(decoded_instruction_buffer, decoded_instruction_buffer_size, symbol_buffer, symbol_buffer_size);
		break;

	case C_POP:
		//write pop instructions
		write_pop(decoded_instruction_buffer, decoded_instruction_buffer_size, symbol_buffer, symbol_buffer_size);
		break;

	case C_LABEL:
		write_label(symbol_buffer);
		break;

	case C_GOTO:
		write_goto(symbol_buffer);
		break;

	case C_IF:
		write_if_goto(symbol_buffer);
		break;

	case C_FUNCTION:
		write_function(decoded_instruction_buffer, symbol_buffer);
		break;

	case C_RETURN:
		write_return();
		break;

	case C_CALL:
		write_call(decoded_instruction_buffer, symbol_buffer);
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