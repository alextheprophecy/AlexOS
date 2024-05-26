#ifndef GRAPHICS_DR_H
#define GRAPHICS_DR_H

#include "../kernel/low_level.h"

#define SCREEN_W 640
#define SCREEN_H 480 
#define VGA_ADDRESS 0xA0000

void _set_bit_mask(unsigned char mask);
void _select_plane(unsigned char plane);
#endif