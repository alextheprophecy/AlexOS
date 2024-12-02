# Mini x86 Kernel

A lightweight x86 kernel implementation with protected mode, interrupt handling, and basic VGA graphics capabilities.

## Features

- Protected Mode Operation
- Interrupt Handling System
  - Custom Interrupt Descriptor Table (IDT)
  - Interrupt Service Routines (ISR)
  - Interrupt Request (IRQ) handling
- VGA Graphics
  - Text mode output
  - Graphics mode with plane-based VGA memory access
  - Basic drawing primitives:
    - Line drawing (Bresenham's algorithm)
    - Rectangle drawing
    - Color support

## how to

nasm boot_sect.asm -f bin -o boot_sect.bin

qemu-system-x86_64 -drive file=boot_sect.bin,format=raw

## functions

print(char* message)

print(int number) !!warning: must be in confines of (MIN_INT, MAX_INT)

## graphical capabilites

draw_line(Point p, color) -uses Bresenham's line algorithm

## resources

 <https://www.kernel.org/doc/gorman/html/understand/understand009.html>
