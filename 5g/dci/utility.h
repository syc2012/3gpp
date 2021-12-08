#ifndef __OSWRAPPER_H__
#define __OSWRAPPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef unsigned char   bool;
#define BOOL_FALSE      0
#define BOOL_TRUE       1

/* Integers */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint24;
typedef unsigned int    uint32;
typedef unsigned long long  uint64;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int24;
typedef signed int      int32;
typedef signed long long    int64;


#define DIV_CEIL(X, Y)  (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y) ((X) / (Y))


/* Frequency domain resource assignement bits */
extern uint8 g_fresBits[276];


int hex_string(char *pStr, uint8 *pBuf, int bufLen);
void mem_dump(void *pAddr, int size);
uint32 get_bits(uint8 *pBuf, int offset, int bits);


#endif
