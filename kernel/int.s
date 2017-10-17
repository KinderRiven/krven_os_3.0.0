[BITS 32]
[global timer_interrupt]
[global move_to_user_mode]
[extern do_timer]

ret_from_sys_call:
	;recover
	pop		eax
	pop		ebx
	pop		ecx
	pop		edx
	pop		fs
	pop		es
	pop		ds
	iret

;iret
;[1].ss
;[2].esp
;[3].psw
;[4].cs
;[5].eip
;[6-12].ds es fs edx ecx ebx eax
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
	;EOF
	mov		al,	0x20
	out		0x20, al	
	;do timer
	mov		ax, [0x20+esp] ; +36
	and		ax, 0x03
	push	ax
	call	do_timer
	add		esp, 2	
	jmp		ret_from_sys_call

;iret
;1.ss
;2.esp
;3.psw
;4.cs
;5.eip
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
