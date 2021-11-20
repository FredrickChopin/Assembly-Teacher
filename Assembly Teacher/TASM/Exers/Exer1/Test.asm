LOCALS @@

sta segment stack
dw 100h dup (?) 
sta ends

data segment
	include Exers/Exer1/Code.asm
	include Exers/Basic.asm
	count db 4
	x1_teacher dw 1, 2, 3, 8
	x2_teacher dw 1, 2, 3, 9
	x3_teacher dw 2, 4, 6, 17
	result_teacher dw ?
data ends

assume cs: code, ds: data ,ss: sta

code segment
@@start:
	mov ax, data
	mov ds, ax
	call test_code_teacher
	add ax, '0'
	push ax
	call print_char_teacher
	mov ax, 4c00h
	int 21h
	
test_code_teacher proc 
	call push_registers_teacher
	mov dx, 1
	mov cl, count
	mov ch, 0
	mov si, 0
	@@loop:
		mov ax, x1_teacher[si]
		mov bx, x2_teacher[si]
		call push_registers_teacher
		call add_and_store 
		mov result_teacher, cx
		call pop_registers_teacher
		mov di, result_teacher
		cmp di, x3_teacher[si]
		jne @@fail
		add si, 2
		loop @@loop
	@@end:
		mov ax, dx
		call pop_registers_teacher
		ret
	@@fail:
		mov dx, 0
		jmp @@end
test_code_teacher endp 

code ends
end @@start