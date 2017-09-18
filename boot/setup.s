[BITS 16]
setup_start:
	jmp	$

setup_fill:
	times 2048 - ($ - $$) db 0x11
