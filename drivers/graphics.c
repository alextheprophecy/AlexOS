#include "graphics.h"
#include "../kernel/low_level.h"
#include "../lib/integer/integer.h"

void draw(){   
    
    draw_debug_vertical_line(64);
    draw_debug_vertical_line(72);
    draw_debug_vertical_line(80);
    draw_debug_vertical_line(88);
    draw_debug_vertical_line(96);
    draw_debug_vertical_line(104);
    draw_debug_vertical_line(112);
    draw_debug_vertical_line(120);

    Point p1 = {62,100};   
    draw_rectangle(p1, 5, 50, GREEN);
    Point p1c = {70,50};
    draw_rectangle(p1c, 1, 50, BLUE);

    //draw_rectangle(50, 100, 400, 300, 0xd);
    Point vertices[3] = { {100, 100}, {200, 300}, {300, 150} };
    

    Point p2 = {64,150};
    Point p3 = {68,200};
    Point p4 = {72,250};
    Point p5 = {74,300};

    draw_rectangle(p2, 16, 50, BLUE);
    draw_rectangle(p3, 16, 50, CYAN);

    draw_rectangle(p4, 12, 50, YELLOW);
    draw_rectangle(p5, 24, 50, BROWN);
}

void draw_debug_vertical_line(int x){
    Point p = {x, 0};
    Point p2 = {x, SCREEN_H};
    draw_line(p,p2,4);
}

// Select the appropriate VGA memory plane
void _select_plane(unsigned char plane) {
    port_byte_out(0x3C4, 0x02); // Index to map mask register
    port_byte_out(0x3C5, 1 << plane); // Set plane
}

// Set the bit mask for writing to a specific pixel in a byte
void _set_bit_mask(unsigned char mask) {
    port_byte_out(0x3CE, 0x08); // Index to bit mask register
    port_byte_out(0x3CF, mask); // Set bit mask
}

void draw_pixelXY(int x, int y, unsigned char color) {
    Point p = {x,y};
    draw_pixel(p, color);
}

void draw_pixel(Point p, unsigned char color) {
    unsigned int offset;
    unsigned char mask, plane_mask;

    // Calculate the offset in the VGA memory for the pixel
    offset = (p.y * SCREEN_W + p.x) >> 3;

    mask = 0x80 >> (p.x % 8);
    _set_bit_mask(mask);
    // Loop over the 4 planes
    for (int plane = 0; plane < 4; plane++) {
        _select_plane(plane);    

        // Read the existing byte from VGA memory
        unsigned char* location = (unsigned char*)VGA_ADDRESS + offset;
        unsigned char value = *location;       
        if (color & (1 << plane)) {
            value |= mask;
        } else {
            value &= ~mask;
        }

        // Write the byte back to VGA memory
        *location = value;
    }
}

/**
 * set bit mask: select which pixels
 * set location: set plane value (1 byte = 8 pixels) each plane is on or off if color
*/
// void draw_horizontal_line(Point p, int width, unsigned char color) {
//     unsigned int offset = (p.y * SCREEN_W + p.x) >> 3;

//     int l_shift = (p.x % 8);
//     int r_shift  = 8-((p.x + width) % 8);

//     unsigned char start_mask = 0xFF >> l_shift;
//     unsigned char end_mask = 0xFF << (7 - ((p.x + width - 1) % 8));
//     unsigned int width_s = width + l_shift;
//     unsigned int width_t = width - (8-p.x % 8);

//     unsigned int tmp2 = ((p.x+width) % 8)==0;

//     for (int plane = 0; plane < 4; plane++) {
//         _select_plane(plane);

//             // Set start byte
//         _set_bit_mask(start_mask);
//         unsigned char* location = (unsigned char*)VGA_ADDRESS + offset;
//         unsigned char value = *location;
//         if (color & (1 << plane)) {
//             value |= start_mask;
//         } else {
//             value &= ~start_mask;
//         }
//         *location = value;


//         if(width_t>8){
//             // Set middle bytes
//             _set_bit_mask(0xFF);
//             for (int i = 8; i < width_s - 8; i += 8) {
//                 location = (unsigned char*)VGA_ADDRESS + offset + (i >> 3);
//                 *location = (color & (1 << plane)) ? 0xFF : 0x00;
//             }
//         }

//         // Set end byte
//         _set_bit_mask(end_mask);
//         location = (unsigned char*)VGA_ADDRESS + offset - tmp2 + (width_s >> 3);
//         value = *location;
//         if (color & (1 << plane)) {
//             value |= end_mask;
//         } else {
//             value &= ~end_mask;
//         }
//         *location = value;
        
//     }
// }

void new_draw_line(Point p, int width, unsigned char color) {
    unsigned int offset = (p.y * SCREEN_W + p.x) >> 3;

    int l_shift = (p.x % 8);
    int r_shift = 8 - ((p.x + width) % 8);

    unsigned char start_mask = 0xFF >> l_shift;
    unsigned char end_mask = 0xFF << r_shift;

    int mid_count = ((width - (8-l_shift) - (8-r_shift)) >> 3); //: get width, and remove the pixels on left and right side (that draw partial byte blocks) and divide by byte size

    for (int plane = 0; plane < 4; plane++) {
            _select_plane(plane);           

            if(width<=8 && (p.x >> 3)==((p.x+width) >> 3)){ //&& (p.x>>3 == (p.x+width)>>3)
                
                _set_bit_mask(start_mask&end_mask);
                unsigned char* location = (unsigned char*)VGA_ADDRESS + offset;
                unsigned char value = *location;
                if (color & (1 << plane)) {
                    value |= start_mask&end_mask;
                } else {
                    value &= ~(start_mask&end_mask);
                }
                *location = value;
            }else{
                _set_bit_mask(start_mask);
                unsigned char* location = (unsigned char*)VGA_ADDRESS + offset;
                unsigned char value = *location;
                if (color & (1 << plane)) {
                    value |= start_mask;
                } else {
                    value &= ~start_mask;
                }
                *location = value;

                // Set middle bytes
                
                _set_bit_mask(0xFF);
                for (int i=0; i < mid_count; ++i){
                    location = (unsigned char*)VGA_ADDRESS + offset + 1+ i;
                    *location = (color & (1 << plane)) ? 0xFF : 0x00;
                }

                _set_bit_mask(end_mask);
                location = (unsigned char*)(VGA_ADDRESS + offset + 1+ mid_count);
                value = *location;
                if (color & (1 << plane)) {
                    value |= end_mask;
                } else {
                    value &= ~end_mask;
                }
                *location = value;
            }
    }

}


void draw_line(Point p1, Point p2, unsigned char color)
{
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

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      draw_pixelXY(px,py,color);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      draw_pixelXY(px,py,color);
    }
  }
}


void draw_rectangle(Point start, int width, int height, unsigned char color) {
    Point p = {start.x, start.y};
    for (int j = 0; j < height; j++) {
        p.y=start.y+j;
        new_draw_line(p, width, color);
    }
}
