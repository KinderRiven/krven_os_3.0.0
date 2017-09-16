[BITS 32]
[global kernel_entry]
section .text
kernel_entry:
	xor	ax,	ax
	xor bx, bx
	xor	cx, cx
	jmp	$
	xor	si, si
	xor di, di

section .data
	times 1024 db 0x11

