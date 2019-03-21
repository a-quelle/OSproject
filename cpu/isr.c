#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "paging.h"
#include "timer.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "ports.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(unsigned char n, isr_t handler){
    interrupt_handlers[n] = handler;
}

void isr_install(){
    set_idt_gate(0, (int)isr0);
    set_idt_gate(1, (int)isr1);
    set_idt_gate(2, (int)isr2);
    set_idt_gate(3, (int)isr3);
    set_idt_gate(4, (int)isr4);
    set_idt_gate(5, (int)isr5);
    set_idt_gate(6, (int)isr6);
    set_idt_gate(7, (int)isr7);
    set_idt_gate(8, (int)isr8);
    set_idt_gate(9, (int)isr9);
    set_idt_gate(10, (int)isr10);
    set_idt_gate(11, (int)isr11);
    set_idt_gate(12, (int)isr12);
    set_idt_gate(13, (int)isr13);
    set_idt_gate(14, (int)isr14);
    set_idt_gate(15, (int)isr15);
    set_idt_gate(16, (int)isr16);
    set_idt_gate(17, (int)isr17);
    set_idt_gate(18, (int)isr18);
    set_idt_gate(19, (int)isr19);
    set_idt_gate(20, (int)isr20);
    set_idt_gate(21, (int)isr21);
    set_idt_gate(22, (int)isr22);
    set_idt_gate(23, (int)isr23);
    set_idt_gate(24, (int)isr24);
    set_idt_gate(25, (int)isr25);
    set_idt_gate(26, (int)isr26);
    set_idt_gate(27, (int)isr27);
    set_idt_gate(28, (int)isr28);
    set_idt_gate(29, (int)isr29);
    set_idt_gate(30, (int)isr30);
    set_idt_gate(31, (int)isr31);

    // Remap the PIC (wiki.osdev/PIC)
    outb(0x20, 0x11); //Send initialise command to master PIC
    outb(0xA0, 0x11); //Send initialise command to slave PIC
    outb(0x21, 0x20); //master PIC interrupts rerouted to isr 32-40
    outb(0xA1, 0x28); //slave PIC interrupts rerouted to isr 41-48
    outb(0x21, 0x04); //tell master that slave is connected to IRQ2
    outb(0xA1, 0x02); //tell slave it is connected to IRQ2
    outb(0x21, 0x01); //tell master PIC it is on a 8086 CPU
    outb(0xA1, 0x01); //tell slave PIC it is on a 8086 CPU
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 

    // Install the IRQs
    set_idt_gate(32, (int)irq0);
    set_idt_gate(33, (int)irq1);
    set_idt_gate(34, (int)irq2);
    set_idt_gate(35, (int)irq3);
    set_idt_gate(36, (int)irq4);
    set_idt_gate(37, (int)irq5);
    set_idt_gate(38, (int)irq6);
    set_idt_gate(39, (int)irq7);
    set_idt_gate(40, (int)irq8);
    set_idt_gate(41, (int)irq9);
    set_idt_gate(42, (int)irq10);
    set_idt_gate(43, (int)irq11);
    set_idt_gate(44, (int)irq12);
    set_idt_gate(45, (int)irq13);
    set_idt_gate(46, (int)irq14);
    set_idt_gate(47, (int)irq15);

    set_idt();
}

void irq_install(){
    asm volatile ("sti"); //interrupts were disabled in switch_pm, reenable them
    init_timer(50);     //timer connected to IRQ0
    init_keyboard();    //keyboard connected to IRQ1
    initialise_paging();//paging connected to IRQ14
}

char* exception_messages[] = {
    //0-8
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    //8-16
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    //17-24
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    //25-32
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

//for OS generated interrupts, only print for now
void isr_handler(registers_t* r){
    if(interrupt_handlers[r->int_no]){
        interrupt_handlers[r->int_no](r);
    }
    kprint("Received Unhandled Interrupt: ");
    char s[3];
    int_to_ascii(r->int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r->int_no]);
    kprint("\n");
}

//For interrupts from PIC, send EOI and call specific handler
void irq_handler(registers_t* r){
    //send EOI to the PICs so they will send new interrupts
    if(r->int_no >= 40) 
        outb(0xa0, 0x20); //do for slave PIC only if used
    outb(0x20, 0x20);

    if(interrupt_handlers[r->int_no]){
        interrupt_handlers[r->int_no](r);
    }
}