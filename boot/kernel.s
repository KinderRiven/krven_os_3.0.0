[BITS 32]

KERNEL_PAGE_NUM		equ	5
PAGE_SIZE			equ	4096

[global kernel_entry]
[global kernel_stack]
[global page_dir]
[global gdt]
[global idt]

[extern kernel_main]
section .text
page_dir:

kernel_entry:
	mov		ax,	0x10
	mov		ds,	ax
	mov		es,	ax
	mov		ss, ax
	mov		eax,gdt
	mov		esp,kernel_stack
	lgdt	[gdt_48]
	lidt	[idt_48]
	sti
	call	kernel_main
	int		0x80
	jmp		$		

times	(4096 * 5)	db	0

section .data
ALIGN	8
gdt_48:
	dw	gdt_length
	dd	gdt
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
	;other
	times	(253 * 8)	db	0
gdt_length	equ		($ - gdt - 1)

idt_48:
	dw	idt_length
	dd	idt
idt:
	times	(8 * 256)	db	0
idt_length	equ		($ - idt - 1)
	times	2048	db	0	
kernel_stack:
