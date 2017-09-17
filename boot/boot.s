[BITS 16]
BOOT_SEG		equ	07c0h
INIT_SEG		equ	9000h
SETUP_SEG		equ	9020h
SETUP_ADDR		equ	0200h
SETUP_NUM		equ	4
SETUP_START		equ	2

boot_start:
	call clean_console
    mov	ax, cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax

;move boot from [07c00h] -> [90000h]
move_boot_init:
	xor	bx,	bx
	mov	ax,	BOOT_SEG
	mov	ds,	ax
	mov	ax,	INIT_SEG
	mov	es,	ax
	mov	cx,	256

move_boot:
	mov	ax, [ds:bx]
	mov	[es:bx], ax
	add	bx,	2
	loop move_boot

mov_boot_ok:
	jmp INIT_SEG:load_setup_init

;prepare to load setup sector
load_setup_init:
	mov	ax,	cs
	mov	ds,	ax
	mov es,	ax
	mov	ss,	ax
	mov	sp, 0xFF00
	
	push setup_message
	push es
	push setup_message_length
	push 0x02
	push 0
	mov	 bp, sp
	call print_message
	add  sp, 10

;start load setup
load_setup:
	push SETUP_ADDR
	push INIT_SEG
	push SETUP_NUM
	push SETUP_START
	push 0
	push 0
	mov	 bp, sp
	call read_sectors
	add	 sp, 12

;get some driver info
;int 0x13
load_setup_ok:
	mov	dl, 0
	mov	ax, 0x0800
	int 0x13

;start load kernel
load_kernel:

;jmp to setup
jmp_setup:
	jmp SETUP_SEG:0

;-------------------------------;
;read sector (head start es:bx)	;
;ah = 0x02 	al = sector number	;
;ch = track cl = sector_start	;
;dh = head 	dl = driver_id		;
;es:bx = read buffer			;
;-------------------------------;
;driver_id = 0					;
;-------------------------------;
;ret_addr	<- sp
;head	(2)	<- ep
;track	(2)
;sector	(2)
;num	(2)
;es 	(2)
;bx 	(2)
;-------------------------------;
read_sectors:
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	mov		dh, [bp]
	mov		dl, 0
	mov		ch, [bp + 2]
	mov		cl, [bp + 4]
	mov		ah, 0x02
	mov		al, [bp + 6]
	mov		es, [bp + 8]
	mov 	bx, [bp + 10]		
	int 	0x13
	pop		es
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret
;--------------------------------;
;es:bp = message_addr
;cx = message_length
;(dh,dl) = start_point
;al = 1
;bl = color
;--------------------------------;
;ret_addr	<- sp
;(dh,dl)	<- bp
;color
;length
;es
;bp
;--------------------------------;
print_message:
	push	ax
	push	bx
	push	cx
	push	dx
	push	bp
	mov		ax, 0x1301
	mov		bx, 0
	mov		dx, [bp]
	mov		bl, [bp + 2]
	mov		cx, [bp + 4]
	mov		es, [bp + 6]
	mov		bp, [bp + 8]
	int		0x10
	pop		bp
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret

clean_console:
	push	ax
	push	bx
	push	cx
	push	dx
	mov		ah, 0x06
	mov		al,	0
	mov		cx, 0
	mov		dh, 25
	mov		dl, 80
	int		0x10		
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret
		
sectors:
	dw	0x0000
setup_message:
	dw	"LOADING SETUP...."
setup_message_length	equ	 ($ - setup_message)

boot_fill:
    times 510 - ($ - $$) db 0
boot_last:
    dw 0xAA55
