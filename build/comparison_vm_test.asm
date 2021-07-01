@420
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@1337
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
@1
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@0
M=M-1 //decrement SP
A=M
D=M
@0
M=M-1 //decrement SP again
A=M
M=D&M //compute AND
@0
M=M+1 //increment SP
@0
M=M-1 //decrement SP
A=M
D=M
@0
M=M-1 //decrement SP again
A=M
D=A-D
@LT_TRUE.0
D;JLT
@LT_FALSE.0
D;JMP
(LT_TRUE.0)
@-1 //boolean true, also 0xFFFF
D=A
@0
A=M
M=D
@LT_END.0
D;JMP
(LT_FALSE.0)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(LT_END.0)
@0
A=M
M=M+1 //incrementing SP
@23
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
M=M-D //sub
@0
M=M+1 //increment SP
@0
M=M-1 //decrement SP
A=M
D=M
@0
M=M-1 //decrement SP again
A=M
M=D|M //compute OR
@0
M=M+1 //increment SP
