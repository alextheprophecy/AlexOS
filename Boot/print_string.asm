print_string:
	mov ah, 0x0e

loop:
	mov al, [bx]
	cmp al, 0
	je out
	int 0x10
	add bx, 0x01
	jmp loop

out:
	ret

print_char:
    pusha
    mov ah, 0x0E
    int 0x10
    popa
    ret