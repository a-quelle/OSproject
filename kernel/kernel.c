#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main(){
    isr_install();
    __asm__ __volatile__ ("sti"); //interrupts were disabled in switch_pm, reenable them
    init_keyboard();
}
