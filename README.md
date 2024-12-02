# Mini x86 Kernel

A bare metal x86 kernel with a custom two-stage bootloader, written primarily in Assembly and C. This project demonstrates low-level system programming concepts by implementing core OS functionality from scratch.

## Architecture Overview

### ⚙️Custom Two-Stage Bootloader
- First Stage (16-bit real mode):
  - Loads from boot sector
  - Sets up initial environment
  - Loads second stage bootloader
- Second Stage:
  - Memory detection via BIOS
  - Switches to 32-bit protected mode
  - Loads and jumps to kernel

### 📋Kernel Features

- Protected Mode Operation with GDT
- Memory Management
  - Custom linker script for memory layout
  - BSS section initialization
  - Text and data segment handling

- Interrupt Handling System
  - Custom Interrupt Descriptor Table (IDT)
  - Interrupt Service Routines (ISR)
  - Hardware interrupt handling (IRQ)
 
- **🖋️Text Mode (text 40*25 16 color)** 
  - Full keyboard support with scan code mapping
  - Screen scrolling and cursor management
  - Basic text editing capabilities
    
- **🖼️Graphics Mode (VGA 320*200 256 color)**
  - Graphics mode with plane-based VGA memory access
  - Basic drawing primitives:
    - Line drawing (Bresenham's algorithm)
    - Rectangle drawing
    - Color support
      
### Memory Layout
- Bootloader: 0x7c00 (First stage), 0x7d00 (Second stage)
- Kernel: 0x1500
- Stack: 0x90000


## Development Tools required
- **GCC Cross-Compiler (i686-elf-gcc)** - Required for compiling C code to run on bare metal x86 hardware
- **NASM** - For assembling bootloader and low-level components
- **QEMU** - For system emulation and testing

## Resources

 <https://www.kernel.org/doc/gorman/html/understand/understand009.html>
 
 <https://wiki.osdev.org/Expanded_Main_Page>
 
 <https://539kernel.com/A_Journey_in_Creating_an_Operating_System_Kernel_The_539kernel_Book.pdf>
 
 Different VGA modes: <https://wiki.osdev.org/Drawing_In_Protected_Mode>
