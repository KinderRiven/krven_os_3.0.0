[BITS 16]
%include	"global.inc"
KERNEL_SIZE			equ	3000h
SETUP_SEG			equ	9020h
INFO_SEG			equ	9000h
KERNEL_TEXT_SELECT	equ	0008h
KERNEL_DATA_SELECT	equ	00F0h
MEM_ADDR			equ	2
;----------------------------;
;
;----------------------------;
setup_start:

;prepare to get info
get_info_init:
	mov		ax, INFO_SEG
	mov		ds,	ax	

;get memory info
get_mem_info:
	mov		ah, 0x88
	int		0x15
	mov		[MEM_ADDR], ax			
	jc		get_info_error

;get info ok
get_info_ok:

;move kernel
do_move_init:
	cli
	mov		cx,	KERNEL_SIZE
	shr		cx, 1
	mov		ax, 0
	mov		es, ax
	mov		ax, 1000h
	mov		ds, ax
	mov		bx, 0

;ds:bx -> 01000h | es:bx -> 00000h
do_move:
	mov		ax, [ds:bx]
	mov		[es:bx], ax
	add		bx, 2
	jc		update_seg
	loop	do_move	

load_gdt_idt:
	mov		ax, SETUP_SEG
	mov		ds,	ax
	lgdt	[gdt_48]
	lidt	[idt_48]

jmp_to_protect:
	in		al, 92h
	or		al,	00000010b
	out		92h,al
	mov		eax, cr0
	or		eax, 1
	mov		cr0, eax
	jmp		dword 8:0	
	jmp		$	

update_seg:
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

get_info_error:
	jmp		$

error_message:
	dw		"GET INFO ERROE"
error_message_length	equ	($ - error_message)

;--------------------------------------------;
gdt:
	;null segment
	dw	0, 0, 0, 0
	;kernel text segment	
	dw	0xFFFF
	dw	0x0000
	dw	0x9A00
	dw	0x00CF
	;kernel	data segment
	dw	0xFFFF
	dw	0x0000
	dw	0x9200
	dw	0x00CF
	
gdt_length	equ	($ - gdt - 1)

gdt_48:
	dw	gdt_length
	dw	512	+ gdt, 0x9
idt_48:
	dw	0, 0, 0
;--------------------------------------------;
setup_fill:
	times 2048 - ($ - $$) db 0x11
