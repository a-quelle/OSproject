#include "timer.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"
#include "isr.h"

int tick = 0;

//timer_callback is of type *isr_t
//
static void timer_callback(registers_t r){
    ++tick;
    kprint("Tick: ");
    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
}

void init_timer(int freq){
    //PIT0 couples to IRQ0
    register_interrupt_handler(IRQ0, timer_callback);

    int divisor = 1193180 / freq;
    char low = (char)(divisor & 0xff);
    char high = (char)((divisor >> 8) & 0xff);
    outb(0x43, 0x36); //write 0x36 (square wave on PIT0, set low and high bit) to PIT command port
    //port 0x40 is PIT0, set low and high bit of frequency divisor
    outb(0x40, low);
    outb(0x40, high); 
}