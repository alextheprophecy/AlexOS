C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c drivers/*.c lib/*/*.c)
HEADERS = $(wildcard kernel/*.h kernel/*/*.c drivers/*.h lib/*/*.h)
TXT_FILES = $(wildcard res/*.txt)

OBJ = $(C_SOURCES:.c=.o) $(TXT_FILES:.txt=.o)


all : os-image
run : all
	qemu-system-i386 -drive format=raw,file=os-image

os-image : boot/first_boot.bin boot/second_boot.bin kernel.bin #boot/boot_sect.bin#boot/boot_sect.bin kernel.bin
	cat $^ > os-image

kernel.bin : kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -T linker.ld -o $@ $^ --oformat binary
#i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o : %.c ${HEADERS} 
	i686-elf-gcc -w -ffreestanding -nostdlib -c $< -o $@

# Assemble the kernel_entry
%.o : %.asm
	nasm $< -f elf -o $@

# res/%.o : res/%.txt
# 	i686-elf-objcopy -I binary -O elf32-i386 -B i386 --rename-section .data=.rodata,alloc,load,data,contents,readonly $< $@
# 	i686-elf-objcopy --redefine-sym _binary_______resources_test_png_start=_binary_test_png_start test_png.o
# #i686-elf-objcopy -I binary -O elf32-i386 -B i386 $< $@


res/%.o : res/%.S
	rm -fr res/%.s
	i686-elf-gcc -x assembler-with-cpp -c $< -o $@


res/%.S : res/%.txt data_template.S.in
	sed -e "s,@sym@,$(basename $(notdir $<)),g" -e "s,@file@,$<," < data_template.S.in > $@



%.bin : %.asm
	nasm $< -f bin -I 'Boot/' -o $@

clean :
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o lib/*/*.o
	rm -fr res/*.S res/*.o


# Disassemble our kernel - might be useful for debugging .
kernel.dis : kernel.bin
	ndisasm -b 32 $< > $@

read_symbols : 
	i686-elf-readelf -s ${ARG} -W