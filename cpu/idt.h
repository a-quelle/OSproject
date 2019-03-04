#ifndef IDT_H
#define IDT_H

#include "types.h"

#define KERNEL_CS 0x08 //code segment in gdt

//similar to code in gdt.asm, but now using c-structs and helper functions

typedef struct{    
    short low_offset; /* Lower 16 bits of handler function address */
    short sel; /* Kernel segment selector */
    char always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    char flags; 
    short high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t;

typedef struct{
    short limit;
    int base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, int handler);
void set_idt();

#endif