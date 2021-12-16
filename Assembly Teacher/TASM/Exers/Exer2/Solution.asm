;Write a procedure called Store_Zero_To_Nine 
;that stores the numbers 0 to 9 (both ends included) into the array a

;Define your stack here
sta segment stack
	dw 100h dup (?)
sta ends

;Define your data here
data segment
	a db 10 dup (0) ;The array 
data ends

code segment

assume cs: code, ds: data, ss: sta

;Your entering point
Store_Zero_To_Nine proc
	mov bx, 0
	@@loop:
	mov a[bx], bl
	inc bx
	cmp bx, 9
	jle @@loop
	ret ;Make sure to return by writing ret, without any number following
Store_Zero_To_Nine endp

;Define here other procedures, if necessary

code ends