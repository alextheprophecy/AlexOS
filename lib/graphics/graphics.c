#include "graphics.h"

void draw(){   

    Point p1 = {62,100};   
    draw_rectangle(p1, 5, 50, GREEN);
    Point p1c = {66,50};
    draw_rectangle(p1c, 4, 50, BLUE);

    //draw_rectangle(50, 100, 400, 300, 0xd);
    Point vertices[3] = { {100, 100}, {400, 100}, {300, 150} };
    

    Point p2 = {64,150};
    Point p3 = {68,200};
    Point p4 = {72,250};
    Point p5 = {74,300};

    draw_rectangle(p2, 16, 50, BLUE);
    draw_rectangle(p3, 16, 50, CYAN);

    draw_rectangle(p4, 12, 50, YELLOW);
    draw_rectangle(p5, 24, 50, BROWN);

    draw_line(vertices[0], vertices[1], RED);
}

void draw_debug_vertical_line(int x){
    Point p = {x, 0};
    Point p2 = {x, SCREEN_H};
    draw_line(p,p2,4);
}

void draw_pixelXY(int x, int y, unsigned char color) {
    Point p = {x,y};
    draw_pixel(p, color);
}

void draw_pixel(Point p, unsigned char colour) {
    unsigned int offset;
    unsigned char mask, plane_mask;

    // Calculate the offset in the VGA memory for the pixel
    offset = (p.y * SCREEN_W + p.x) >> 3;

    mask = 0x80 >> (p.x % 8);
    _set_bit_mask(mask);
    // Loop over the 4 planes
    for (int plane = 0; plane < 4; plane++) {
        _select_plane(plane);    
        unsigned char* location = (unsigned char*)VGA_ADDRESS + offset;
        unsigned char value = *location;       
        if (colour & (1 << plane)) value |= mask;
        else value &= ~mask;        
        *location = value;    }
}

//could be even faster by allocating a precomputed array of VGA memory offsets before looping through the 4 planes
void draw_horizontal_line(Point p, int width, unsigned char colour) {
    unsigned char* vga_start = (unsigned char*)(VGA_ADDRESS + ((p.y * SCREEN_W + p.x) >> 3));

    int l_shift = (p.x % 8);
    int r_shift = 8 - ((p.x + width) % 8);

    unsigned char start_mask = 0xFF >> l_shift;
    unsigned char end_mask = 0xFF << r_shift;

    int mid_count = ((width - (8-l_shift) - (8-r_shift)) >> 3); //: get width, and remove the pixels on left and right side (that draw partial byte blocks) and divide by byte size

    for (int plane = 0; plane < 4; plane++) {
            _select_plane(plane);
            if(width<=8 && (p.x >> 3)==((p.x+width) >> 3)){ //&& (p.x>>3 == (p.x+width)>>3)  
                _set_plane_mask_colour(colour, plane, start_mask&end_mask, (unsigned char*)vga_start);

            }else{
                //set first byte
                _set_plane_mask_colour(colour, plane, start_mask, (unsigned char*)vga_start);

                //set middle bytes                
                _set_bit_mask(0xFF);
                for (int i=0; i < mid_count; ++i){
                    unsigned char* location = (unsigned char*)(vga_start + 1 + i);
                    *location = (colour & (1 << plane)) ? 0xFF : 0x00;
                }

                //set last byte
                _set_plane_mask_colour(colour, plane, end_mask, (unsigned char*)(vga_start + 1 + mid_count));
            }
    }

}

void _set_plane_mask_colour(unsigned char colour, int plane, unsigned char mask, unsigned char* location){
    _set_bit_mask(mask);
    unsigned char value = *location;
    if (colour & (1 << plane)) value |= mask;
    else value &= ~mask;    
    *location = value;
}

void draw_line(Point p1, Point p2, unsigned char colour){  
    if(p1.y==p2.y){
        if(p1.x==p2.x)return draw_pixel(p1, colour);
        else return draw_horizontal_line(p1, abs(p2.x - p1.x),colour);
    }

    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

    dx=p2.x-p1.x;      /* the horizontal distance of the line */
    dy=p2.y-p1.y;      /* the vertical distance of the line */
    dxabs=abs(dx);
    dyabs=abs(dy);
    sdx=sign(dx);
    sdy=sign(dy);
    x=dyabs>>1;
    y=dxabs>>1;
    px=p1.x;
    py=p1.y;

    if (dxabs>=dyabs){ /* the line is more horizontal than vertical */  
        for(i=0;i<dxabs;i++){    
            y+=dyabs;
            if (y>=dxabs){
                y-=dxabs;
                py+=sdy;
            }
            px+=sdx;
            draw_pixelXY(px,py,colour);
        }
    }else{ /* the line is more vertical than horizontal */
        for(i=0;i<dyabs;i++){
            x+=dxabs;
            if (x>=dyabs){
                x-=dyabs;
                px+=sdx;
            }
            py+=sdy;
            draw_pixelXY(px,py,colour);
        }
    }
}

void draw_rectangle(Point start, int width, int height, unsigned char colour) {
    Point p = {start.x, start.y};
    for (int j = 0; j < height; j++) {
        p.y=start.y+j;
        draw_horizontal_line(p, width, colour);
    }
}
