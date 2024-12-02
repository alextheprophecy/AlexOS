[org 0x7d00]

KERNEL_OFFSET equ 0x1500
SECOND_BOOT_SECTOR_SIZE equ 5
mov [BOOT_DRIVE], dl

mov bx, MSG_TWO ; Announce that we are starting
call print_string ; booting from 16 - bit real mode


call print_memory_map ; Print the memory map

;call load_kernel
;call switch_to_pm

%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov ax, 2+SECOND_BOOT_SECTOR_SIZE
    mov bx, KERNEL_OFFSET
    mov dh, 19
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

print_memory_map:
    call print_newline
    mov bx, MSG_MEMORY_MAP
    call print_string
    call print_newline

    mov di, 0x1000       ; Point ES:DI to the buffer in low memory
    mov es, di
    xor di, di

    mov ebx, 0           ; Continuation value, initially 0
    mov ecx, 0x20        ; Size of the buffer

memory_map_loop:
    mov ax, 0xE820
    mov edx, 0x534D4150  ; 'SMAP'
    int 0x15
    jc memory_map_done   ; If CF is set, we're done

    call print_memory_entry

    cmp ebx, 0           ; If ebx is 0 after the call, we're done                                                                                           
    je memory_map_done

    jmp memory_map_loop

memory_map_done:
    ret

print_memory_entry:
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    mov bx, es:[di+2]    ; Base address mid
    call print_hex
    mov bx, es:[di]      ; Base address low
    call print_hex
    
    call print_seperator

    mov bx, es:[di+10]   ; Length mid
    call print_hex
    mov bx, es:[di+8]    ; Length low
    call print_hex

    call print_seperator

    mov bx, es:[di+16]   ; Type
    call print_hex

    call print_newline
    add di, 0x14         ; Move to the next memory map entry

    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret

print_hex:
    pusha
    mov cx, 4            ; Print 4 digits (16 bits)
print_hex_digit:
    rol bx, 4            ; Rotate left to bring the next nibble into the lowest 4 bits
    mov al, bl           ; Load the lowest 4 bits
    and al, 0x0F         ; Mask out the upper 4 bits
    add al, '0'          ; Convert to ASCII
    cmp al, '9'
    jbe print_hex_output
    add al, 7            ; Adjust for A-F

print_hex_output:
    call print_char
    loop print_hex_digit
    popa
    ret

print_seperator:
    mov bx, SEPERATOR
    call print_string
    ret

print_newline:
    mov al, 0xd
    call print_char
    mov al, 0xa
    call print_char
    ret

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0
MSG_TWO db "Second boot. ", 0
MSG_LOAD_KERNEL db " Loading kernel into memory. ", 0
MSG_MEMORY_MAP db "Memory map:", 0
SEPERATOR db " - ", 0

times (SECOND_BOOT_SECTOR_SIZE*512) - ($ - $$) db 0

; Ensure you have the following functions defined in print_string.asm:
; print_string, print_char
