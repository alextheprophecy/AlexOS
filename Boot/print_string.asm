[bits 16]
print_string:
	mov ah, 0x0e

print_string_loop:
	mov al, [bx]
	cmp al, 0
	je print_string_out

	int 0x10
	add bx, 0x01
	jmp print_string_loop

print_string_out:
	mov al, ' '
	int 0x10
	ret