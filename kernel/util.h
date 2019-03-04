#ifndef UTIL_H
#define UTIL_H

void memory_copy(char* source, char* dest, int nbytes);

void memory_set(char* dest, char val, int len);

void int_to_ascii(int n, char str[]);

char* reverse(char* str);

int strlen(char* s);

#endif