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


const char push_constant[] = "D=A\n@0 //SP\nA=M\nM=D\n@0 //SP\nM=M+1\n";


#endif