;This file contains basic procedures for handling input and output of tests

STACK_OFFSET = 18

data segment
	file_handle dw ?
	file_name db "Output.txt", 0
	var_next_instruction dw ?
	msg0 db '0'
	msg1 db '1'
data ends

code segment 
assume cs: code, ds: data ,ss: sta

;To open file:
; ah = 3dh
; dx = offset file name
; al = {0 read, 1 write, 2 both}
;int 21h
; file handle will be in ax
; possible errors:
; 2 - file not found
; 5 - too many files open
; 12 - access denied

;To write to file:
; ah = 40h
; bx = file handle
; cx = number of bytes to write
; dx = offset buffer
; ax will be set to the numbers of bytes written, or 5 if accsses denied 

;To close a file:
; ah = 3eh

print_to_file proc 
	;To use this, push offset of msg
	call push_registers
	call open_file
	mov bp, sp
	mov dx, [bp + STACK_OFFSET]
	mov ah, 40h
	mov bx, file_handle
	mov cx, 1
	int 21h
	call pop_registers
	ret 2
print_to_file endp 

open_file proc 
	call push_registers
	mov dx, offset file_name
	mov ah, 3dh
	mov al, 1
	int 21h
	mov file_handle, ax
	call pop_registers
	ret
open_file endp

create_file proc
	;First, change file attributes
	call push_registers
	mov dx, offset file_name
	mov ah, 3ch
	mov cx, 1
	int 21h
	call pop_registers
	ret
create_file endp

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