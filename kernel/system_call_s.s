[BITS 32]
[global timer_interrupt]
[global system_call]
[global move_to_user_mode]
[global sys_test]
[global sys_fork]
[extern do_timer]
[extern sys_call_table]
[extern find_empty_process]
[extern copy_process]
[extern schedule]

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

reschedule:
	push	ret_from_sys_call
	jmp		schedule

;[1-5] ss esp psw cs eip (int push)
;[] ds es fs edx ecx ebx eax
;[] retaddr
;[]	gs esi edi ebp nr 
sys_fork:
	call	find_empty_process	
	cmp		eax, -1
	je		.1
	push	gs
	push	esi
	push	edi
	push	ebp
	push	eax
	call	copy_process
	add		esp, 20
.1:
	ret

sys_test:
	ret

system_call:
	;save
	push	ds
	push	es
	push	fs
	push	edx
	push	ecx
	push	ebx
	mov		dx, 0x10
	mov		ds,	dx
	mov		es,	dx
	mov		dx, 0x17
	mov		fs,	dx
	call	[sys_call_table+eax*4]
	push	eax
	;return
	jmp		reschedule
	
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
