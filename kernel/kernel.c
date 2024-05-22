#include "../drivers/screen.h"
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

void main() {   
    clear_bss();
    idt_install();
    isr_install();
	irq_install();
    keyboard_install();
    //timer_install();

    __asm__ __volatile__("sti");    
    clear_screen();  
    print("Alex OS\nWelcome!");   
    draw();   
}