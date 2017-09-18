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
	times 0x1000 - ($ - $$) db 0x12
section .data
	times 0x1000 - ($ - $$) db 0x34

