#include "../drivers/screen.h"
#include "kernel.h"
#include "../drivers/keyboard.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/paging.h"
#include "../libc/string.h"
#include "../libc/mem.h"
void kernel_main(){
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
    if(strcmp(input, "PAGE") == 0){
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, PAGE, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n> ");
        return;
    }
    if(strcmp(input, "INT8") == 0){
        uint32_t phys_addr;
        uint32_t page = kmalloc(1, UNALIGNED, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n> ");
        return;
    }
    if(strcmp(input, "INT16") == 0){
        uint32_t phys_addr;
        uint32_t page = kmalloc(2, INT16, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n> ");
        return;
    }
    if(strcmp(input, "INT32") == 0){
        uint32_t phys_addr;
        uint32_t page = kmalloc(4, INT32, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n> ");
        return;
    }
    if(strcmp(input, "INT64") == 0){
        uint32_t phys_addr;
        uint32_t page = kmalloc(8, INT64, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n> ");
        return;
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}