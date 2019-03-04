#ifndef TYPES_H
#define TYPES_H

#define low_16(integer) (short)((integer) & 0xFFFF)
#define high_16(integer) (short)(((integer) >> 16) & 0xFFFF)

#endif