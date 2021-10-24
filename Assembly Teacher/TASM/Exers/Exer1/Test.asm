LOCALS @@

sta segment stack
dw 100h dup (?) 
sta ends

data segment
	include Exers/Exer1/Code.asm
	include Exers/Basic.asm
	count db 4
	x1 dw 1, 2, 3, 8
	x2 dw 1, 2, 3, 9
	x3 dw 2, 4, 6, 17
	result dw ?
data ends

assume cs: code, ds: data ,ss: sta

code segment
@@start:
	mov ax, data
	mov ds, ax
	call test_code
	add ax, '0'
	push ax
	call print_char
	mov ax, 4c00h
	int 21h
	
test_code proc 
	call push_registers
	mov dx, 1
	mov cl, count
	mov ch, 0
	mov si, 0
	@@loop:
		mov ax, x1[si]
		mov bx, x2[si]
		call push_registers
		call add_and_store 
		mov result, cx
		call pop_registers
		mov di, result
		cmp di, x3[si]
		jne @@fail
		add si, 2
		loop @@loop
	@@end:
		mov ax, dx
		call pop_registers
		ret
	@@fail:
		mov dx, 0
		jmp @@end
test_code endp 

code ends
end @@start