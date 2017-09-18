[BITS 16]
INFO_KERNEL			equ	9000h
KERNEL_TEXT_SELECT	equ	0008h
KERNEL_DATA_SELECT	equ	00F0h
MEM_ADDR			equ	2
;----------------------------;
;
;----------------------------;
setup_start:
	mov		ax,	INFO_KERNEL
	mov		ds,	ax


get_mem_info:
	mov		ah, 0x88
	int		0x15
	mov		[MEM_ADDR], ax			
	jc		get_info_error

get_info_error:
	jmp		$

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

error_message:
	dw		"GET INFO ERROE"
error_message_length	equ	($ - error_message)

setup_fill:
	times 2048 - ($ - $$) db 0x11
