LOADING_MESSAGE db "RUNNING BOOT SECTOR..."
_start:
    mov ax, 0xFF
    mov bx, 0xFF
    mov cx, 0xFF
    jmp $
_print:

_fill:
    times 510 - ($ - $$) db 0
_last:
    dw 0xAA55