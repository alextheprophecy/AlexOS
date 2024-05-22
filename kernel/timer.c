#include "low_level.h"
int timer_Hz = 100;

void setup_timer_phase(int hz){
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    port_byte_out(0x43, 0x36);             /* Set our command byte 0x36 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
}
	
/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

void timer_handler(struct regs *r){
    timer_ticks++;
    if (timer_ticks % timer_Hz == 0){
        printn("One second has passed\n");
    }
}

void timer_install(){
    setup_timer_phase(timer_Hz); //
    irq_install_handler(0, timer_handler);
}	

void pause(int seconds){
    unsigned long eticks;
    eticks = timer_ticks + seconds * timer_Hz;

    while(timer_ticks < eticks);
}