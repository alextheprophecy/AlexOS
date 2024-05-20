all : os-image
# Run bochs to simulate booting of our code .
run : all
	qemu-system-i386 -drive format=raw,file=os-image
# This is the actual disk image that the computer loads ,
# which is the combination of our compiled bootsector and kernel
os-image : boot_sect.bin kernel.bin
	cat $^ > os-image

# This builds the binary of our kernel from two object files :
# - the kernel_entry , which jumps to main () in our kernel
# - the compiled C kernel
kernel.bin : kernel_entry.o kernel.o
	i686-elf-ld -T linker.ld -o $@ $^ --oformat binary
#i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

# Build our kernel object file .
kernel.o : kernel.c
	i686-elf-gcc -ffreestanding -c $< -o $@

# Build our kernel entry object file .
kernel_entry.o : kernel_entry.asm
	nasm $< -f elf -o $@

boot_sect.bin : Boot/boot_sect.asm
	nasm $< -f bin -I 'Boot/' -o $@
# Clear away all generated files .
clean :
	rm -fr *.bin *.dis *.o os-image *.map
# Disassemble our kernel - might be useful for debugging .
kernel.dis : kernel .bin
	ndisasm -b 32 $< > $@