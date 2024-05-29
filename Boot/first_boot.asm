[org 0x7c00]
SECOND_BOOT_OFFSET equ 0x7d00

mov [BOOT_DRIVE], dl
mov bp, 0x20 ; Set -up the stack.
mov sp, bp
mov bx, MSG ; Announce that we are starting
call print_string ; booting from 16 - bit real mode

mov ax, 2
mov bx , SECOND_BOOT_OFFSET
mov dh , 1
mov dl , [BOOT_DRIVE]
call disk_load

jmp SECOND_BOOT_OFFSET

jmp $

%include "print_string.asm"
%include "disk_load.asm"

BOOT_DRIVE db 0
MSG db "First. " , 0

; Bootsector padding
times 510 -($ -$$) db 0
dw 0xAA55