boot_addr		equ	07c0h
init_addr		equ	9000h
setup_sectors	equ	4

boot_start:
    mov	ax, cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax

;move boot from [07c00h] -> [90000h]
move_boot_init:
	xor	bx,	bx
	mov	ax,	boot_addr
	mov	ds,	ax
	mov	ax,	init_addr
	mov	es,	ax
	mov	cx,	256

move_boot:
	mov	ax, [ds:bx]
	mov	[es:bx], ax
	add	bx,	2
	loop move_boot
	jmp init_addr:jmp_90000h	

;jmp to [90000h]
jmp_90000h:
	mov	ax,	cs
	mov	ds,	ax
	mov es,	ax
	mov	ss,	ax

;prepare to load setup sector
load_setup_init:

;start load
load_setup:				
	mov	ax,	0x0200 + setup_sectors
	mov	cx, 0x0002
	mov	dx,	0
	mov	bx,	0200h
	int 0x13

;jmp to setup
jmp_setup:
	jmp	init_addr:0x0200
	

boot_fill:
    times 510 - ($ - $$) db 0
boot_last:
    dw 0xAA55
