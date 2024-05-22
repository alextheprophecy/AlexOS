C_SOURCES = $(wildcard drivers/*.c lib/*/*.c kernel/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h lib/*/*.h)
OBJ = $(C_SOURCES:.c=.o)	


all : os-image
run : all
	qemu-system-i386 -drive format=raw,file=os-image

os-image : boot/boot_sect.bin kernel.bin
	cat $^ > os-image

kernel.bin : kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -T linker.ld -o $@ $^ --oformat binary
#i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary


%.o : %.c ${HEADERS} 
	i686-elf-gcc -ffreestanding -nostdlib -c $< -o $@

# Assemble the kernel_entry
%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -I 'Boot/' -o $@

clean :
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o lib/*/*.o

# Disassemble our kernel - might be useful for debugging .
kernel.dis : kernel.bin
	ndisasm -b 32 $< > $@