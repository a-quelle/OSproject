#include "idt.h"

void set_idt_gate(int n, int handler){
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8e;
    idt[n].high_offset = high_16(handler);
}

void set_idt(){
    idt_reg.base = (int) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidtl (%0)": : "r" (&idt_reg));
}