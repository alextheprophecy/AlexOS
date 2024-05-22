#include "keyboard.h"
#include "low_level.h"

int isShiftPressed = 0;

const char kbd[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x2,
    '\\', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, 0, '+', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r){
    unsigned char scancode;
    scancode = port_byte_in(0x60);

    // this means a key has been released
    if (scancode & 0x80){
        if (kbd[scancode & 0x7f]==0x2) isShiftPressed = 0; // scancode and ~(0x80) (remove the release bit to lookup in table)
    }else{       
        if (kbd[scancode]==0x2) isShiftPressed = 1;
        else{
            char c = kbd[scancode];
            if (isShiftPressed==1 && is_char_alphabetical(c)) printch(c-0x20);
            else printch(c);            
        }
    }
}

void keyboard_install(){
    irq_install_handler(1, keyboard_handler);
}	