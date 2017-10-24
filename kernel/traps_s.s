[BITS 32]
[global divide_error]
[global page_error]
[global tss_error]
[extern do_page_error]

no_error_code:
	push	0
	jmp		$
	mov		eax, 0

error_code:
	jmp		$	
	mov		eax, 1


tss_error:
	jmp		$
	mov		eax, 2

;DE-0
divide_error:
	jmp		no_error_code
	jmp		$
	mov		eax, 3

;PF-14
page_error:
	jmp		$
	jmp		do_page_error
	jmp		$
	mov		eax, 4
