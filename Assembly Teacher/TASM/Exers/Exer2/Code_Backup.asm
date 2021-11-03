;Write a function called add_and_store that stores the sum of ax and bx into cx

LOCALS @@

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
add_and_store proc

	ret ;Make sure to return by writing ret, without any number following
add_and_store endp

;Define here other procedures, if necessary

code ends