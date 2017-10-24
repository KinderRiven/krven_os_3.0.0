[BITS 32]
KERNEL_PAGE_NUM		equ	5
PAGE_SIZE			equ	4096

[global kernel_entry]
[global task0_stack_top]
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
	mov		esp,task0_stack_top
	lgdt	[gdt_48]
	lidt	[idt_48]
	jmp		kernel_jmp
		
fill:
	times	PAGE_SIZE - ($ - $$)	db	0
page_0:
	times	PAGE_SIZE	db	0x11
page_1:
	times	PAGE_SIZE	db	0x22
page_3:
	times	PAGE_SIZE	db	0x33
page_4:
	times	PAGE_SIZE	db	0x44

kernel_jmp:
	call	kernel_main
	jmp		$
	push	eax
	jmp		$
	push	ebx
	jmp		$
	push	ecx

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
	times	((256 - 3) * 8)	db	0
gdt_length	equ		($ - gdt - 1)

idt_48:
	dw	idt_length
	dd	idt
idt:
	times	(8 * 256)	db	0
idt_length	equ		($ - idt - 1)

task0_stack_bottom:
	times	4096	db	0	
task0_stack_top:
