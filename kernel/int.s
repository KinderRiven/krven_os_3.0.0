[BITS 32]
[global timer_interrupt]
[global move_to_user_mode]
[extern do_timer]
;iret
;1.eip
;2.cs
;3.psw
;4.esp
;5.ss

timer_interrupt:
	;save
	push	ds
	push	es
	push	fs
	push	edx
	push	ecx
	push	ebx
	push	eax
	mov		eax, 0x10
	mov		ds,	ax
	mov		es,	ax
	mov		eax, 0x17
	mov		fs,	ax
	;do timer
	mov		ax, cs
	and		ax, 0x03
	push	ax
	call	do_timer
	add		esp, 2	
	;recover
	pop		eax
	pop		ebx
	pop		ecx
	pop		edx
	pop		fs
	pop		es
	pop		ds
	;EOF
	mov		al,	0x20
	out		0x20, al	
	iret

move_to_user_mode:	
	mov		eax, esp
	;ss
	mov		ebx, 0x17
	push	ebx
	;esp
	push	eax
	;psw
	pushf
	;cs
	mov		ebx, 0x0f
	push	ebx
	;eip			
	push	ret_addr
	iret
ret_addr:
	mov		eax, 0x17
	mov		ds,	ax
	mov		es,	ax
	mov		fs,	ax
	mov		gs,	ax
	ret
