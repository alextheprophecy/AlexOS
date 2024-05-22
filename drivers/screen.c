#include "screen.h"
#include "../kernel/low_level.h"
#include "../kernel/memory.h"
#include "../lib/integer/integer.h"

void print_char(char character , int col , int row , char attribute_byte) {   
    if (character == 0) return;

    unsigned char * vidmem = (unsigned char *) VIDEO_ADDRESS ;

    if (!attribute_byte) attribute_byte = WHITE_ON_BLACK ;
    
    int offset;
    if (col >= 0 && row >= 0) offset = get_screen_offset(col, row);
    else offset = get_cursor();      

    vidmem[offset] = character;
    vidmem[offset + 1] = 0;  
    
    int next_offset = offset;

    if (character == '\n'){            //new line              
        int rows = offset / (2 * MAX_COLS);
        next_offset = get_screen_offset(80, rows);    

    }else if (character == '\b'){       //backspace 
        vidmem[next_offset] = ' ';
        if(offset>0){         
            next_offset -= 2;             
            while(next_offset>0 && vidmem[next_offset] ==' ') next_offset -= 2;
            char t = vidmem[next_offset];
            if(t!='\n' && t!='\t') next_offset = offset-2;
            vidmem[next_offset] = ' ';     
        }   

    }else if (character == '\t'){       //tab                 
        next_offset += TAB_SIZE*2;
    }else{    
        vidmem[offset+1] = attribute_byte;    
        next_offset += 2;
    }

    vidmem[next_offset + 1] = attribute_byte;

    offset = handle_scrolling(next_offset);
    set_cursor(offset);
}

int get_screen_offset(int col, int row){
    return 2 * (row * MAX_COLS + col);
}

int get_cursor(){
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA)<<8;  // Read the high byte of the cursor's offset
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);  // Read the low byte of the cursor's offset

    return offset * 2;
}

void set_cursor(int offset){
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset>>8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset&0xFF));
}

void print_at(char* message, int col, int row){
    if (col >= 0 && row >= 0) set_cursor(get_screen_offset(col, row));
    
    int i = 0;
    while (message[i] != 0) 
        print_char(message[i++], col, row, WHITE_ON_BLACK);    
}

void print(char* message){
    print_at(message, -1, -1);
}

void printint(int number){
    char str[32];
    int_to_str(number, str);
    print(str);
}

void printch(char * character){  
    unsigned char * vidmem = (unsigned char *) VIDEO_ADDRESS ;  
    vidmem[get_cursor()] = character;

    print_char(character, -1, -1, WHITE_ON_BLACK);    
}


void printn(char* message){
    print_char('\n', -1, -1, WHITE_ON_BLACK); 
    print(message);
}


void clear_screen() {
    int row, col; 
    for (row = 0; row < MAX_ROWS; row++) {
        for (col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }
    set_cursor(get_screen_offset(0, 0));
}


int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS *2) return cursor_offset ;
    
    int i;
    for (i =1; i < MAX_ROWS ; i ++) 
        memcpy((char*)get_screen_offset(0 , i) + VIDEO_ADDRESS, (char*)get_screen_offset(0, i -1) + VIDEO_ADDRESS, MAX_COLS *2);

    char* last_line = (char*)get_screen_offset(0 , MAX_ROWS -1) + VIDEO_ADDRESS;
    for (i =0; i < MAX_COLS *2; i ++)
        last_line [i] = 0;

    cursor_offset -= 2* MAX_COLS;
    return cursor_offset ;
}
