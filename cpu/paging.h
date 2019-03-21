#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include "isr.h"

#define INDEX_FROM_FRAME(a) (uint32_t)(a/32)
#define OFFSET_FROM_FRAME(a) (uint32_t)(a%32)

typedef struct page{ //use uint32 because bitfield totals 32 bit
    uint32_t present : 1;
    uint32_t writable : 1;
    uint32_t user : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t unused : 7;
    uint32_t frame : 20;
} __attribute__((packed)) page_t;

typedef struct page_table{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory{
    page_table_t* tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physicalAdress;
} page_directory_t;

void initialise_paging();

void switch_page_directory(page_directory_t* dir);

page_t* get_page(uint32_t address, int make, page_directory_t* dir);

void page_fault_handler(registers_t* r);

#endif