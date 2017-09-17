
setup_start:
	mov	ax,	0xffff
	mov	bx,	0xffff
	mov	cx,	0xffff	
	jmp	$
	xor	di,	di
	xor	si,	si

setup_fill:
	times 2048 - ($ - $$) db 0x11
