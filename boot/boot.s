boot_addr		equ	07c0h
init_addr		equ	9000h

boot_start:
    mov	ax, cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax
	jmp	$
move_init:
	xor	bx,	bx
	mov	ax,	boot_addr
	mov	ds,	ax
	mov	ax,	init_addr
	mov	es,	ax
	mov	cx,	256
	
do_move:
	mov	ax, [ds:bx]
	mov	[es:bx], ax
	add	bx,	2
	loop do_move
	jmp init_addr:move_9000h	

move_9000h:
	mov	ax,	cs
	mov	ds,	ax
	mov es,	ax
	mov	ss,	ax	
	jmp	$

boot_fill:
    times 510 - ($ - $$) db 0
boot_last:
    dw 0xAA55
