#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SCREEN_W 640
#define SCREEN_H 480 
#define VGA_ADDRESS 0xA0000

//colours:
#define BLACK 00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define WHITE 0x07
#define GRAY 0x08
#define L_BLUE 0x09
#define L_GREEN 0x0a
#define L_CYAN 0x0b
#define L_RED 0x0c
#define L_MAGENTA 0x0d
#define YELLOW 0x0e
#define BR_WHITE 0x0f

typedef struct {
    int x;
    int y;
} Point;



void draw();
void draw_pixel(Point p, unsigned char color);
void draw_horizontal_line(Point p, int width, unsigned char color);
void draw_rectangle(Point p, int width, int height, unsigned char color);

void _set_bit_mask(unsigned char mask);
void _select_plane(unsigned char plane);
#endif