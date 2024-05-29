[org 0x7d00]
KERNEL_OFFSET equ 0x1500
SECOND_BOOT_SECTOR_SIZE equ 2
mov [BOOT_DRIVE], dl

mov bx, MSG_TWO ; Announce that we are starting
call print_string ; booting from 16 - bit real mode
call load_kernel

mov bx, MSG_TWO ; Announce that we are starting
call print_string ; booting from 16 - bit real mode

call switch_to_pm

%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"

[bits 16]
load_kernel :
	mov bx, MSG_LOAD_KERNEL
	call print_string
	
    mov ax, 2+SECOND_BOOT_SECTOR_SIZE
	mov bx , KERNEL_OFFSET
	mov dh , 19
	mov dl , [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
BEGIN_PM :
	call KERNEL_OFFSET
	jmp $


BOOT_DRIVE db 0
MSG_TWO db "Second stage boot" , 0
MSG_LOAD_KERNEL: db " Loading kernel into memory. ", 0
times (SECOND_BOOT_SECTOR_SIZE*512) - ($ - $$) db 0