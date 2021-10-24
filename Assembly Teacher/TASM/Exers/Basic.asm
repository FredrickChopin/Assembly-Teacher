;This file contains basic procedures for handling input and output of tests

STACK_OFFSET = 18

data segment
	var_next_instruction dw ?
data ends

code segment 
assume cs: code, ds: data ,ss: sta

print_char proc
	call push_registers
	mov bp, sp
	mov dx, [bp + STACK_OFFSET]
	mov ah, 2h
	int 21h
	call pop_registers
	ret 2
print_char endp

push_registers proc
	pop var_next_instruction
	irp register, <bx, cx, dx, ds, es, di, si, bp>
		push register
	endm
	push var_next_instruction
	ret
push_registers endp

pop_registers proc
	pop var_next_instruction
	irp register, <bp, si, di, es, ds, dx, cx ,bx>
		pop register
	endm
	push var_next_instruction
	ret
pop_registers endp

code ends