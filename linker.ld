ENTRY(_start)
SECTIONS
{
    . = 0x1500;
    .text ALIGN(4) : {
        *(.multiboot)
        *(.text)
    }    
    .data ALIGN(4) : {
        __data_load = LOADADDR(.data);
        __data_start = .;
        *(.data)
        __data_end = .;
    }
    .rodata ALIGN(4) : {
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
    }
    .bss ALIGN(4) : {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    }
    .text_data ALIGN(4) : {
        __text_data_start = .;  
        *(.text_data)
        __text_data_end = .;
    }
    __k_heap_start = .;
}