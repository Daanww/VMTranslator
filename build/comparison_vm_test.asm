@THIS
D=M
@2
A=D+A //compute location we want to access
D=M
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@THAT
D=A
@1
D=D+A
@temp_this_that
M=D //storing THIS/THAT+x for use later
@0 //SP
M=M-1
A=M
D=M
@temp_this_that
A=M
M=D
