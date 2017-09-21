[BITS 32]
[global divide_error]
[global page_error]

no_error_code:
	jmp		$

;DE-0
divide_error:
	jmp		no_error_code
	jmp		$

error_code:
	jmp		$	

;PF-14
page_error:
	jmp		error_code
	jmp		$
