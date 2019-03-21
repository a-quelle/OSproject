#include "paging.h"
#include "../libc/mem.h"
#include "../libc/string.h"
#include "../drivers/screen.h"

extern uint32_t free_mem_addr;
uint32_t* frames = NULL;
uint32_t nframes = 0;
page_directory_t* current_directory = NULL;

static void alloc_frame(page_t* page, int is_kernel, int is_writable);
//static void free_frame(page_t* page);

page_t* get_page(uint32_t address, int make, page_directory_t* dir){
    address /= 0x1000;
    uint32_t table_index = address / 1024;
    uint32_t page_index = address % 1024;
    if(dir->tables[table_index])
        return &dir->tables[table_index]->pages[page_index];
    else if(make){
        uint32_t phys_addr = NULL;
        dir->tables[table_index] = (page_table_t*) kmalloc(sizeof(page_table_t), PAGE, &phys_addr);
        memory_set((uint8_t*)(dir->tables[table_index]), 0, 0x1000);
        dir->tablesPhysical[table_index] = phys_addr | 0x7; //present, writable, user
        return &dir->tables[table_index]->pages[page_index];
    }
    return 0;
}

void switch_page_directory(page_directory_t* dir){
    current_directory = dir;
    asm volatile ("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void initialise_paging(){
    uint32_t mem_end_page = 0x1000000;
    nframes = mem_end_page / 0x1000;

    frames = (uint32_t*) kmalloc(nframes/8, INT32, NULL);
    memory_set((uint8_t*)frames, 0, nframes/8);

    current_directory = (page_directory_t*) kmalloc(sizeof(page_directory_t), PAGE, NULL);
    memory_set((uint8_t*)current_directory, 0, sizeof(page_directory_t));
    
    uint32_t i = 0;
    while(i < 0x100000){   //Allocate a frame for first MiB, for written code + VGA memory
        alloc_frame(get_page(i,1,current_directory), 0, 1);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault_handler); //page faults come from ISR14
    switch_page_directory(current_directory);
}

void page_fault_handler(registers_t* r){
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0": "=r"(faulting_address));

    int present = !(r->err_code & 1);   //page not present?
    int write = r->err_code & 2;        //caused by write?
    int user = r->err_code & 4;         //in user mode?
    int reserved = r->err_code & 8;     //overwritten cpu-reserved bits of page entry?
    int id = r->err_code & 0x10;        //caused by instruction fetch?

   kprint("Page fault! ( ");
   if (present) {kprint("not present ");}
   if (write) {kprint("read-only ");}
   if (user) {kprint("user-mode ");}
   if (reserved) {kprint("reserved ");}
   if (id) {kprint("instruction ");}
   kprint(") at 0x");
   char hexString[16] = "";
   hex_to_ascii((int)faulting_address, hexString);
   kprint(hexString);
   kprint("\n");
   asm volatile ("hlt");
}

/**********************************
 *  Frame functions implementation
 * *******************************/
static void set_frame(uint32_t frame){
    uint32_t index = INDEX_FROM_FRAME(frame);
    uint32_t offset = OFFSET_FROM_FRAME(frame);
    frames[index] |= (0x1 << offset); 
}
// static void clear_frame(uint32_t frame){
//     uint32_t index = INDEX_FROM_FRAME(frame);
//     uint32_t offset = OFFSET_FROM_FRAME(frame);
//     frames[index] &= ~(0x1 << offset);     
// }
// static uint32_t test_frame(uint32_t frame){
//     uint32_t index = INDEX_FROM_FRAME(frame);
//     uint32_t offset = OFFSET_FROM_FRAME(frame);
//     return frames[index] & (0x1 << offset);  
// }

static uint32_t first_free_frame(){
    uint32_t i, j;
    for(i = 0; i < INDEX_FROM_FRAME(nframes); ++i){
        if(frames[i] != 0xFFFFFFFF) {
            for(j = 0; j < 32; ++j){
                if(!(frames[i] & (0x1 << j)))
                    return 32*i + j;
            }
        }
    }
    return (uint32_t)-1;
}

static void alloc_frame(page_t* page, int is_kernel, int is_writable){
    if(page->present) return;
    uint32_t frame = first_free_frame();
    if(frame == (uint32_t)-1) {
        kprint("No more frames!");
        return;
    }
    set_frame(frame);
    page->present = 1;
    page->user = is_kernel? 0:1;
    page->writable = is_writable? 1:0;
    page->frame = frame;
}

// static void free_frame(page_t* page){
//     if(!page->present) return;
//     clear_frame(page->frame);
//     page->present = 0;
//     page->frame = 0;
// }