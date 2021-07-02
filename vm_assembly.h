#ifndef VM_ASSEMBLY
#define VM_ASSEMBLY

#include <string.h>

/*arithmetic commands
	A_ADD,
	A_SUB,
	A_NEG,
	A_EQ,
	A_GT,
	A_LT,
	A_AND,
	A_OR,
	A_NOT, */

//asm instructions for basic arithmetic operations
const char add[] = "@0\nM=M-1 //decrement SP \nA=M\nD=M //store top most entry in stack in D\n@0\nM=M-1 //decrement SP again\nA=M\nM=D+M //add\n@0\nM=M+1 //increment SP\n";
const char sub[] = "@0\nM=M-1 //decrement SP\nA=M\nD=M //store top most entry in stack in D\n@0\nM=M-1 //decrement SP again\nA=M\nM=M-D //sub\n@0\nM=M+1 //increment SP\n";
const char neg[] = "@0\nM=M-1 //decrement SP\nA=M\nD=M\n@0\nD=A-D //compute -x\nA=M\nM=D\n@0\nM=M+1 //increment SP again\n";


//asm instructions for comparison operations
const char comparison_start[] = "@0\nM=M-1 //decrement SP\nA=M\nD=M\n@0\nM=M-1 //decrement SP again\nA=M\nD=M-D\n";
const char comparison_true[] = "@0 //boolean true, also 0xFFFF\nD=A\nD=D-1\n@0\nA=M\nM=D\n";
const char comparison_false[] = "@0 //boolean false, also 0x0\nD=A\nA=M\nM=D\n";
const char comparison_end[] = "@0\nM=M+1 //incrementing SP\n";
const char comparison_labels[][10] = {	"EQ_TRUE", "EQ_FALSE", "EQ_END", 
										"GT_TRUE", "GT_FALSE", "GT_END", 
										"LT_TRUE", "LT_FALSE", "LT_END"	};



//asm instructions for bitwise operations
const char and[] = "@0\nM=M-1 //decrement SP\nA=M\nD=M\n@0\nM=M-1 //decrement SP again\nA=M\nM=D&M //compute AND\n@0\nM=M+1 //increment SP\n";
const char or[] = "@0\nM=M-1 //decrement SP\nA=M\nD=M\n@0\nM=M-1 //decrement SP again\nA=M\nM=D|M //compute OR\n@0\nM=M+1 //increment SP\n";
const char not[] = "@0\nM=M-1 //decrement SP\nA=M\nM=!M //compute NOT\n@0\nM=M+1 //increment SP\n";

const char push_local_1[] = "@LCL\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push local 2
const char push_argument_1[] = "@ARG\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push argument 2
const char push_local_argument_2[] = "A=D+A //compute location we want to access\nD=M\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n";
const char pop_local_1[] = "@LCL\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push local 2
const char pop_argument_1[] = "@ARG\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push argument 2
const char pop_local_argument_2[] = "D=D+A\n@temp_lcl_arg\nM=D //storing LCL/ARG+x for use later\n@0 //SP\nM=M-1\nA=M\nD=M\n@temp_lcl_arg\nA=M\nM=D\n";

const char push_static[] = "D=M\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n"; //needs an @Xxx\n in front of it to function
const char pop_static_1[] = "@0\nM=M-1\nA=M\nD=M\n"; //requires an @Xxx\n in the middle to function
const char pop_static_2[] = "M=D\n";

const char push_constant[] = "D=A\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n";

const char push_this_1[] = "@THIS\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push this 2
const char push_that_1[] = "@THAT\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push this 2
const char push_this_that_2[] = "A=D+A //compute location we want to access\nD=M\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n";
const char pop_this_1[] = "@THIS\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push this 2
const char pop_that_1[] = "@THAT\nD=M\n"; //requires a @x in the middle with x being the second argument, eg the 2 in push this 2
const char pop_this_that_2[] = "D=D+A\n@temp_this_that\nM=D //storing THIS/THAT+x for use later\n@0 //SP\nM=M-1\nA=M\nD=M\n@temp_this_that\nA=M\nM=D\n";


const char push_pointer[] = "D=M\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n"; //requires a @3+x in front of it to function, x being the x in push pointer x
const char pop_pointer_1[] = "@0\nM=M-1\nA=M\nD=M\n"; //requires a @3+x in the middle
const char pop_pointer_2[] = "M=D\n";

const char push_temp[] = "D=M\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n"; //requires a @3+x in front of it to function, x being the x in push temp x
const char pop_temp_1[] = "@0\nM=M-1\nA=M\nD=M\n"; //requires a @3+x in the middle
const char pop_temp_2[] = "M=D\n";

#endif