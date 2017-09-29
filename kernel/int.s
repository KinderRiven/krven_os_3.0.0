[BITS 32]
[global timer_interrupt]
[global move_to_user_mode]

;iret
;1.eip
;2.cs
;3.psw
;4.esp
;5.ss

timer_interrupt:
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
