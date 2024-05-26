#include "kernel.h"

extern char __rodata_start;
extern char __rodata_end;

void clear_bss() {
    extern char __bss_start;
    extern char __bss_end;
    char *bss = &__bss_start;
    while (bss < &__bss_end) {
        *bss++ = 0;
    }
}

void main(int* count, char* args) {   
    clear_bss();
    idt_install();
    isr_install();
	irq_install();
    keyboard_install();
    //timer_install();

    __asm__ __volatile__("sti");    
   
    clear_screen();  
    print("Hello and welcome\n");
    printint(*count);
    printch(args);

    printn("INFO:\n");

    unsigned short total;
    unsigned char lowmem, highmem;
 
    port_byte_out(0x70, 0x30);
    lowmem = port_byte_in(0x71);
    port_byte_out(0x70, 0x31);
    highmem = port_byte_in(0x71);
 
    total = lowmem | highmem << 8;
    printhex(total);
    //draw();  

}