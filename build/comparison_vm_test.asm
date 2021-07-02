@LCL
D=M
@2
A=D+A //compute location we want to access
D=M
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@ARG
D=M
@5
D=D+A
@temp_lcl_arg
M=D //storing LCL/ARG+x for use later
@0 //SP
M=M-1
A=M
D=M
@temp_lcl_arg
A=M
M=D
