;Write a procedure called add_and_store that stores the sum of ax and bx into cx

;Define your stack here
sta segment stack
	dw 100h dup (?)
sta ends

;Define your data here
data segment

data ends

code segment

assume cs: code, ds: data, ss: sta

;Your entering point

;This is one of the possible solutions
add_and_store proc
	add ax, bx
	mov cx, ax
	ret ;Make sure to return by writing ret, without any number following
add_and_store endp

;Define here other procedures, if necessary

code ends