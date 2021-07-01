@17
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@17
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
@EQ_TRUE.0
D;JEQ
@EQ_FALSE.0
D;JMP
(EQ_TRUE.0)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@EQ_END.0
D;JMP
(EQ_FALSE.0)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(EQ_END.0)
@0
M=M+1 //incrementing SP
@17
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@16
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
@EQ_TRUE.1
D;JEQ
@EQ_FALSE.1
D;JMP
(EQ_TRUE.1)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@EQ_END.1
D;JMP
(EQ_FALSE.1)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(EQ_END.1)
@0
M=M+1 //incrementing SP
@16
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@17
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
@EQ_TRUE.2
D;JEQ
@EQ_FALSE.2
D;JMP
(EQ_TRUE.2)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@EQ_END.2
D;JMP
(EQ_FALSE.2)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(EQ_END.2)
@0
M=M+1 //incrementing SP
@892
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@891
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
@LT_TRUE.0
D;JLT
@LT_FALSE.0
D;JMP
(LT_TRUE.0)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
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
M=M+1 //incrementing SP
@891
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@892
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
@LT_TRUE.1
D;JLT
@LT_FALSE.1
D;JMP
(LT_TRUE.1)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@LT_END.1
D;JMP
(LT_FALSE.1)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(LT_END.1)
@0
M=M+1 //incrementing SP
@891
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@891
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
@LT_TRUE.2
D;JLT
@LT_FALSE.2
D;JMP
(LT_TRUE.2)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@LT_END.2
D;JMP
(LT_FALSE.2)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(LT_END.2)
@0
M=M+1 //incrementing SP
@32767
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@32766
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
@32766
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@32767
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
@GT_TRUE.1
D;JGT
@GT_FALSE.1
D;JMP
(GT_TRUE.1)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@GT_END.1
D;JMP
(GT_FALSE.1)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(GT_END.1)
@0
M=M+1 //incrementing SP
@32766
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@32766
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
@GT_TRUE.2
D;JGT
@GT_FALSE.2
D;JMP
(GT_TRUE.2)
@0 //boolean true, also 0xFFFF
D=A
D=D-1
@0
A=M
M=D
@GT_END.2
D;JMP
(GT_FALSE.2)
@0 //boolean false, also 0x0
D=A
A=M
M=D
(GT_END.2)
@0
M=M+1 //incrementing SP
@57
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@31
D=A
@0 //SP
A=M
M=D
@0 //SP
M=M+1
@53
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
@112
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
D=A-D //compute -x
A=M
M=D
@0
M=M+1 //increment SP again
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
@82
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
M=D|M //compute OR
@0
M=M+1 //increment SP
@0
M=M-1 //decrement SP
A=M
M=!M //compute NOT
@0
M=M+1 //increment SP
