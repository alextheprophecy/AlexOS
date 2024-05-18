mov ah , 0x0e
mov al , 'H'
int 0x10
mov al , 'e'
int 0x10
mov al , 'y'
int 0x10   
jmp $ 


times 510 -( $ - $$ ) db 0 
dw 0xaa55

;nasm boot_sect.asm -f bin -o boot_sect.bin
;qemu-system-x86_64 -drive file=boot_sect.bin,format=raw