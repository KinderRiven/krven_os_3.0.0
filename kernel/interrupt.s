[BITS 32]
[global timer_interrupt]

timer_interrupt:
	jmp		$
	push	ds
	push	es
	push	fs
	push	edx
	push	ecx
	push	ebx
	push	eax
