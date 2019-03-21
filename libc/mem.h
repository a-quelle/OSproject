#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void memory_copy(uint8_t* source, uint8_t* dest, int nbytes);

void memory_set(uint8_t* dest, uint8_t val, int len);

#define UNALIGNED 0
#define PAGE 1
#define INT8 2
#define INT16 3
#define INT32 4
#define INT64 5

uint32_t kmalloc(size_t size, int align, uint32_t* phys_addr);
#endif