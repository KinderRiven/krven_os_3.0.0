[BITS 32]
[global divide_error]
[global page_error]
[extern do_page_error]

no_error_code:
	mov		eax, 0x1
	jmp		$

;DE-0
divide_error:
	mov		eax, 0x2
	jmp		no_error_code
	jmp		$

error_code:
	mov		eax, 0x3
	jmp		$	

;PF-14
page_error:
	jmp		$
	jmp		do_page_error
	jmp		$
