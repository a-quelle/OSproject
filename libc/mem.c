#include "mem.h"

void memory_copy(uint8_t* source, uint8_t* dest, int nbytes){
    int i;
    for(i = 0; i < nbytes; ++i)
        *(dest + i) = *(source + i);
}

void memory_set(uint8_t* dest, uint8_t val, int len){
    for ( ; len; --len) 
        *(dest++) = val;    
}

uint32_t free_mem_addr = 0x8000; //Hardcoded for now. Linker puts kernel at 0x1000

uint32_t kmalloc(size_t size, int align, uint32_t* phys_addr){
    if(align == PAGE && (free_mem_addr & 0x00000FFF)){
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    if(align == INT16 && (free_mem_addr & 0x00000001)){
        free_mem_addr &= 0xFFFFFFFE;
        free_mem_addr += 0x2;
    }
    if(align == INT32 && (free_mem_addr & 0x00000003)){
        free_mem_addr &= 0xFFFFFFFC;
        free_mem_addr += 0x4;
    }
    if(align == INT64 && (free_mem_addr & 0x00000007)){
        free_mem_addr &= 0xFFFFFFF8;
        free_mem_addr += 0x8;
    }
    if(phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}