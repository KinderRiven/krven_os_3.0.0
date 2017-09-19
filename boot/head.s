[BITS 32]
[global kernel_start]
section .text
kernel_start:
	xor	ax,	ax
	xor bx, bx
	xor	cx, cx
	jmp	$

section .data
	times 1024 db 0x11

