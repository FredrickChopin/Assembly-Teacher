;This file contains basic procedures for handling input and output of tests

STACK_OFFSET = 18

data segment
	next_instruction_teacher dw ?
data ends

code segment 
assume cs: code, ds: data ,ss: sta

print_char_teacher proc
	call push_registers_teacher
	mov bp, sp
	mov dx, [bp + STACK_OFFSET]
	mov ah, 2h
	int 21h
	call pop_registers_teacher
	ret 2
print_char_teacher endp

push_registers_teacher proc
	pop next_instruction_teacher
	irp register, <bx, cx, dx, ds, es, di, si, bp>
		push register
	endm
	push next_instruction_teacher
	ret
push_registers_teacher endp

pop_registers_teacher proc
	pop next_instruction_teacher
	irp register, <bp, si, di, es, ds, dx, cx ,bx>
		pop register
	endm
	push next_instruction_teacher
	ret
pop_registers_teacher endp

code ends