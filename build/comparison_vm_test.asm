@comparison_vm_test.0
D=M
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@5
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
D=M-D
@GT_TRUE.0
D;JGT
@GT_FALSE.0
D;JMP
(GT_TRUE.0)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@GT_END.0
D;JMP
(GT_FALSE.0)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(GT_END.0)
@0
M=M+1 //incrementing SP
@0
M=M-1
A=M
D=M
@comparison_vm_test.2
M=D
