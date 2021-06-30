@7
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@8
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@0
M=M-1 //decrement SP 
A=M
D=M //store top most entry in stack in D
@0
M=M-1 //decrement SP again
A=M
M=D+M //add
@0
M=M+1 //increment SP
