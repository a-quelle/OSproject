#include "../drivers/screen.h"
#include "kernel.h"
#include "../drivers/keyboard.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/string.h"
void main(){
    isr_install();
    irq_install();
    kprint("Type something.\nType END to halt the cpu.\n"
            "Caution: buffer length is 3 rows\n> ");
}

void user_input(char* input){
    if(strcmp(input, "END") == 0){
        kprint("Stopping the cpu.\n");
        __asm__ __volatile__ ("hlt");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}