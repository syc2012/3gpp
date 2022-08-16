#ifndef __KDF_H__
#define __KDF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define BYTE_ARRAY_TO_UINT16(p, n) \
    do { \
        n  = (*((uint8*)p  ) <<  8); \
        n |= (*((uint8*)p+1)      ); \
    } while(0)

#define UINT16_TO_BYTE_ARRAY(n, p) \
    do { \
        *((uint8*)p  ) = ((n >>  8) & 0xFF); \
        *((uint8*)p+1) = ((n      ) & 0xFF); \
    } while (0)

#define BYTE_ARRAY_TO_UINT32(p, n) \
    do { \
        n  = (*((uint8*)p  ) << 24); \
        n |= (*((uint8*)p+1) << 16); \
        n |= (*((uint8*)p+2) <<  8); \
        n |= (*((uint8*)p+3)      ); \
    } while (0)

#define UINT32_TO_BYTE_ARRAY(n, p) \
    do { \
        *((uint8*)p  ) = ((n >> 24) & 0xFF); \
        *((uint8*)p+1) = ((n >> 16) & 0xFF); \
        *((uint8*)p+2) = ((n >>  8) & 0xFF); \
        *((uint8*)p+3) = ((n      ) & 0xFF); \
    } while (0)


#define BOOL_FALSE  (0)
#define BOOL_TRUE   (1)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/*  integers  */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   uint64;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;
typedef signed long     int64;

typedef unsigned char   bool;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void xor(uint8 *c, uint8 *a, uint8 *b, int size);
void dump(char *name, const void *addr, int len);

void fips186_2_prng(
    uint8 *XKEY,
    int    XKEYlen,
    uint8 *XSEEDj,
    int    XSEEDjlen,
    uint8 *x,
    int    xlen
);

uint32 kdf_ck_ik_prime(
    uint8  *CK,
    uint8  *IK,
    char   *SNN,
    uint8  *AUTN,
    uint8  *out,
    uint32  outLen
);

void PRF_prime(
    uint8  *key,
    char   *string,
    uint8  *out,
    uint32  outLen
);


#endif /* __KDF_H__ */
