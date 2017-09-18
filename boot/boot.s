[BITS 16]
BOOT_SEG		equ	07c0h
INIT_SEG		equ	9000h
SETUP_SEG		equ	9020h
SETUP_ADDR		equ	0200h
SETUP_NUM		equ	4
SETUP_START		equ	2
KERNEL_SEG		equ	1000h
KERNEL_ADDR		equ	0
KERNEL_SIZE		equ	3000h
KERNEL_START	equ	6

boot_start:
	call	clean_console
    mov		ax, cs
	mov		ds,	ax
	mov		es,	ax
	mov		ss,	ax
;-----------------------------------;
;move boot from [07c00h] -> [90000h];
;-----------------------------------;
move_boot_init:
	xor		bx,	bx
	mov		ax,	BOOT_SEG
	mov		ds,	ax
	mov		ax,	INIT_SEG
	mov		es,	ax
	mov		cx,	256

move_boot:
	mov		ax, [ds:bx]
	mov		[es:bx], ax
	add		bx,	2
	loop 	move_boot

mov_boot_ok:
	jmp 	INIT_SEG:load_setup_init

;-----------------------------------;
;prepare to load setup sector		;
;-----------------------------------;
load_setup_init:
	mov		ax,	cs
	mov		ds,	ax
	mov		es,	ax
	mov		ss,	ax
	mov		sp, 0xFF00
	
	push	setup_message
	push	es
	push	setup_message_length
	push	0x02
	push	0
	mov		bp, sp
	call	print_message
	add 	sp, 10

;start load setup
load_setup:
	push	SETUP_ADDR
	push	INIT_SEG
	push	SETUP_NUM
	push	SETUP_START
	push	0
	push	0
	mov		bp, sp
	call	read_sectors
	add		sp, 12

;get some driver info
;int 0x13
load_setup_ok:
	mov		dl, 0
	mov		ah, 0x08
	int		0x13
	mov		ch, 0
	mov		[sectors_per_track], cx

;-----------------------------------;
;start load kernel					;
;-----------------------------------;
load_kernel_init:
	;print kernel message
	push	kernel_message
	push	INIT_SEG
	push	kernel_message_length
	push	0x02
	push	0x0100
	mov		bp, sp
	call	print_message
	add		sp, 10
	;init
	mov		cx, KERNEL_SIZE
	shr		cx,	9
	inc		cx
	jmp		load_kernel_ok3

load_kernel:
	call	read_sectors
	add		sp, 12

load_kernel_ok0:
	mov		ax, [current_addr]
	add		ax, 512
	mov		[current_addr], ax
	;if not > 0x1000 (64K)
	jnc		load_kernel_ok1
	jmp		$

load_kernel_ok1:
	mov		ax, [current_sector]
	inc		ax
	mov		[current_sector], ax
	cmp		ax, [sectors_per_track]
	jna		load_kernel_ok3
	;sectors > 18 (sector = 19)
	mov		ax,	[current_head]
	cmp		ax, 1
	je		load_kernel_ok2
	;sectors > 18 head = 0
	mov		ax, 1
	mov		[current_head], ax
	mov		[current_sector], ax
	jmp		load_kernel_ok3

load_kernel_ok2:
	;sectors > 18 head = 1
	mov		ax, [current_track]
	inc		ax
	mov		[current_track], ax
	mov		ax, 0
	mov		[current_head], ax
	mov		ax, 1
	mov		[current_sector], ax
	jmp		load_kernel_ok3

load_kernel_ok3:
	mov		ax, [current_addr]
	push	ax
	mov		ax, [current_seg]
	push	ax
	push 	1
	mov		ax, [current_sector]
	push	ax
	mov		ax, [current_track]
	push	ax
	mov 	ax, [current_head]
	push	ax
	mov		bp, sp
	loop	load_kernel
	add 	sp, 12

;jmp to setup
jmp_setup:
	mov		ax, [current_sector]
	mov		bx, [current_track]
	mov		cx, [current_head]
	jmp		SETUP_SEG:0

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
	;print a '.'
	mov	ah, 0x0E
	mov	al, '.'
	mov	bl, 0x02
	int	0x10	
	;start read
	mov		dh, [bp]		; <-- head
	mov		dl, 0
	mov		ch, [bp + 2]	; <-- track
	mov		cl, [bp + 4]	; <-- sector
	mov		ah, 0x02
	mov		al, [bp + 6]
	mov		es, [bp + 8]
	mov 	bx, [bp + 10]		
	int 	0x13
	jc		read_error
	pop		es
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret

read_error:
	jmp		$
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
	;move cursor to (0, 0)
	mov		bh, 0
	mov		dx, 0
	mov		ah, 0x02
	int		0x10
	;clean console
	mov		bh, 0
	mov		al, ' '
	mov		cx, 25 * 80
	mov		ah,0x0a
	int		0x10
	;move cursor to (0, 0)
	mov		bh, 0
	mov		dx, 0
	mov		ah, 0x02
	int		0x10
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret

current_seg:
	dw 0x1000
current_addr:
	dw 0
current_sector:
	dw 6
current_track:
	dw 0
current_head:
	dw 0
	
sectors_per_track:
	dw	0

setup_message:
	dw	"LOADING SETUP"
setup_message_length	equ	 ($ - setup_message)

kernel_message:
	dw "LOADING KERNEL"
kernel_message_length	equ	 ($ - kernel_message)

boot_fill:
    times 510 - ($ - $$) db 0
boot_last:
    dw 0xAA55
