LOCALS @@

sta segment stack
dw 100h dup (?) 
sta ends

data segment
	include Exers/Exer2/Code.asm
	include Exers/Basic.asm
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
	call Store_Zero_To_Nine
	mov ax, 1
	mov bx, 0
	@@loop:
		cmp a[bx], bl
		jne @@fail
		inc bx
		cmp bx, 9
		jg @@end
		jmp @@loop
	@@end:
	call pop_registers_teacher
	ret
	@@fail:
	mov ax, 0
	jmp @@end
test_code_teacher endp 

code ends
end @@start