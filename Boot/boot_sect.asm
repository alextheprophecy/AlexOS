[org 0x7c00]
KERNEL_OFFSET equ 0x1000
mov [BOOT_DRIVE], dl
mov bp, 0x9000 ; Set -up the stack.
mov sp, bp
mov bx, MSG_REAL_MODE ; Announce that we are starting
call print_string ; booting from 16 - bit real mode
call load_kernel
call switch_to_pm ;we wont return from this
jmp $

; Include our useful , hard - earned routines
%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 16]
load_kernel :
	mov bx, MSG_LOAD_KERNEL
	call print_string
	; mov bx , 0x7c00 ; Indirectly set ES to 0 xa000
	; mov es , bx
	
	mov bx , KERNEL_OFFSET
	mov dh , 19; load ceil([ls -l kernel.bin]/512) sectors
	mov dl , [BOOT_DRIVE] 
	call disk_load
	ret

[bits 32]
BEGIN_PM :
	mov ebx, MSG_PROT_MODE ; Use our 32 - bit print routine to
	call print_string_pm ; announce we are in protected mode
	call KERNEL_OFFSET
	jmp $ ; Hang.


BOOT_DRIVE db 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode " , 0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0
MSG_LOAD_KERNEL: db " Loading kernel into memory. ", 0
; Bootsector padding
times 510 -($ -$$) db 0
dw 0xAA55


; times 3*128 dw 0xDADA
