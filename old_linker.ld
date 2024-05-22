
ENTRY(_start)
phys = 0x1000;

SECTIONS
{
   . = phys;
    .text BLOCK(4) : {
        __text_start = .;
        *(.multiboot)
        *(.text)
    }
    .rodata BLOCK(4) : ALIGN(4) {
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
    }
    .data BLOCK(4) : ALIGN(4) {
        __data_start = .;
        *(.data)
        __data_end = .;
    }
    .bss BLOCK(4) : ALIGN(4) {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    }
    __end = .;
}
