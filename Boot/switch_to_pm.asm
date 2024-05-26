[bits 16]
switch_to_pm:
	;set VGA video mode ;lookup table https://wiki.osdev.org/Drawing_In_Protected_Mode
	;mov ax, 0x0002 ;text mode	
	mov ax, 0x0002 ;VGA 320*200 256 color video mode
	int 0x10

	;mem
 	; Gather memory map using BIOS interrupt 0x15, function 0xE820
    xor eax, eax
    mov ax, 0xE820
    mov ecx, 24  ; size of the buffer for each entry
    mov edx, 0x534D4150  ; 'SMAP' signature
    xor ebx, ebx  ; continuation value, initially 0
    mov edi, 0x9000  ; higher memory location for buffer
	call next_region
	;mem

	cli	; We must switch of interrupts until we have
		; set -up the protected mode interrupt vector
		; otherwise interrupts will run riot.

	lgdt [gdt_descriptor]
	
	mov eax, cr0	; see we are using 32 register (eax) here in 16 bit, we can do that
	or eax, 0x1
	mov cr0, eax
	
	jmp CODE_SEG:init_pm

next_region:
    int 0x15
    jc .done  ; carry flag set on error

    ; Check if we are done
    cmp ebx, 0
    jz .done

    add edi, 24  ; move to the next entry in the buffer
    jmp next_region

.done:
    ; Pass the memory map address to the kernel
    mov edi, 0x9000  ; address of the memory map in higher memory
	ret


[bits 32]
init_pm:
	mov ax, DATA_SEG	 ; Now in PM , our old segments are meaningless ,
	mov ds, ax		 ; so we point our segment registers to the
	mov ss, ax		 ; data selector we defined in our GDT
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call BEGIN_PM