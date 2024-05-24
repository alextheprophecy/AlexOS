# how to

nasm boot_sect.asm -f bin -o boot_sect.bin

qemu-system-x86_64 -drive file=boot_sect.bin,format=raw

## functions

print(char* message)

print(int number) !!warning: must be in confines of (MIN_INT, MAX_INT)

## graphical capabilites

draw_line(Point p, color) -uses Bresenham's line algorithm
