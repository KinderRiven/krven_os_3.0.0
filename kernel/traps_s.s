[BITS 32]
[global divide_error]
[global page_error]
[global protect_error]
[global tss_error]
[extern do_error]

;DE-0
divide_error:
	push	0
	push	0
	call	do_error

;TS-10
tss_error:
	mov		eax, 10
	push	eax
	call	do_error
;
protect_error:
	mov		eax, 13
	push	eax
	call	do_error
;PF-14
page_error:
	mov		eax, 14
	push	eax
	call	do_error
