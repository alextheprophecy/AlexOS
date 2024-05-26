#include "graphics_driver.h"

//select the appropriate VGA memory plane
void _select_plane(unsigned char plane) {
    port_byte_out(0x3C4, 0x02); // Index to map mask register
    port_byte_out(0x3C5, 1 << plane); // Set plane
}

//set the bit mask for writing to a specific pixel in a byte
void _set_bit_mask(unsigned char mask) {
    port_byte_out(0x3CE, 0x08); // Index to bit mask register
    port_byte_out(0x3CF, mask); // Set bit mask
}
