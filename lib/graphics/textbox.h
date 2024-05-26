#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "graphics.h" // Assume you have graphics functions defined here

typedef struct {
    Point pos;
    int width;
    int height;   
    unsigned char background_color;
    unsigned char font_color;
    char text[256];
} Textbox;

Textbox* new_textbox(int x, int y, int width, int height, unsigned char background_color, unsigned char font_colour, char* string);
void draw_textbox(Textbox *textbox);
void destroy_textbox(Textbox *textbox);

#endif