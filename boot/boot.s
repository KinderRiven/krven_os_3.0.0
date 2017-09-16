boot_seg		equ	07c0h
init_seg		equ	9000h
setup_seg		equ	9020h
setup_addr		equ	0200h
setup_num		equ	4
setup_start		equ	2

boot_start:
    mov	ax, cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax

;move boot from [07c00h] -> [90000h]
move_boot_init:
	xor	bx,	bx
	mov	ax,	boot_seg
	mov	ds,	ax
	mov	ax,	init_seg
	mov	es,	ax
	mov	cx,	256

move_boot:
	mov	ax, [ds:bx]
	mov	[es:bx], ax
	add	bx,	2
	loop move_boot
	jmp init_seg:load_setup_init	

;prepare to load setup sector
load_setup_init:
	;stack top is 0x9FF00
	mov	ax,	cs
	mov	ds,	ax
	mov es,	ax
	mov	ss,	ax
	mov	sp, 0xFF00

;start load
load_setup:	
	push word setup_addr
	push word init_seg
	push word setup_num
	push word setup_start
	mov	 bp, sp
	call read_sector
	sub	 sp, 8

;jmp to setup
jmp_setup:
	jmp	setup_seg:0

;read sector (start_sector sector_number es bx)
;ah = 0x02 al = sector number
;ch = cidao cl = shanqu
;dh = citouhao dl = qudongqi
;es:bx = read buffer
read_sector:
	mov cx, [bp]
	mov	ch, 0
	mov	ax, [bp + 2]
	mov	ah, 0x02
	mov es, [bp + 4]
	mov bx, [bp + 6]
	mov	dx, 0
	int 0x13
	ret

boot_fill:
    times 510 - ($ - $$) db 0
boot_last:
    dw 0xAA55
