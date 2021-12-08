#include "utility.h"


/* Frequency domain resource assignement bits
 *  = CEIL( log2(N_DL_BWP_RB * (N_DL_BWP_RB + 1) / 2) )
 */
uint8 g_fresBits[276] = {
  0,
  0,  2,  3,  4,  4,  5,  5,  6,  6,  6,
  7,  7,  7,  7,  7,  8,  8,  8,  8,  8,
  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,
  9, 10, 10, 10, 10, 10, 10, 10, 10, 10,
 10, 10, 10, 10, 11, 11, 11, 11, 11, 11,
 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
 11, 11, 11, 12, 12, 12, 12, 12, 12, 12,
 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 15, 15, 15, 15, 15, 16, 16, 16, 16, 16,
 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
 16, 16, 16, 16, 16
};


static int _hex2dec(char c)
{
    int val = 0;

    switch (c)
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            val = (c - '0');
            break;
        case 'a': case 'A':
            val = 10;
            break;
        case 'b': case 'B':
            val = 11;
            break;
        case 'c': case 'C':
            val = 12;
            break;
        case 'd': case 'D':
            val = 13;
            break;
        case 'e': case 'E':
            val = 14;
            break;
        case 'f': case 'F':
            val = 15;
            break;
        default:
            ;
    }

    return val;
}

int hex_string(char *pStr, uint8 *pBuf, int bufLen)
{
    int i;
    int j;

    memset(pBuf, 0, bufLen);

    for (i=0, j=0; i<strlen(pStr); i++)
    {
        if (i & 0x1)
        {
            pBuf[j] |= (_hex2dec( pStr[i] ) & 0xF);
            j++;
        }
        else
        {
            pBuf[j] |= (_hex2dec( pStr[i] ) << 4);
        }
    } 

    return DIV_CEIL(strlen(pStr), 2);
}

void mem_dump(void *pAddr, int size)
{
    uint8 *pByte = pAddr;
    int i;

    if ( pByte )
    {
        for (i=0; i<size; i++)
        {
            if ((i != 0) && ((i % 16) == 0))
            {
                printf("\n");
            }
            printf(" %02X", pByte[i]);
        }
        printf("\n");
        printf(" (%d bytes)\n", size);
        printf("\n");
    }
}

uint32 get_bits(uint8 *pBuf, int offset, int bits)
{
    uint32 value = 0;
    uint8  byte;
    uint8  mask;
    int Q[2];
    int R[2];
    int size[2];
    int shift;
    int loop;
    int i;
    int j;


    Q[0] = ((offset + bits) >> 3);
    R[0] = ((offset + bits) & 0x7);

    Q[1] = (offset >> 3);
    R[1] = (offset & 0x7);

    loop = (Q[0] + ((R[0] > 0) ? 1 : 0));

    size[0] = 8;
    size[1] = bits;

    for (i=Q[1], j=0; j<loop; i++, j++)
    {
        if ((R[1] + size[1]) > 8)
        {
            size[0] = (8 - R[1]);
            shift = 0;
        }
        else
        {
            size[0] = size[1];
            shift = (8 - (R[1] + size[0]));
        }
        mask = ((0x1 << size[0]) - 1);
        byte = ((pBuf[i] >> shift) & mask);

        size[1] -= size[0];
        value |= (byte << size[1]);

        if (0 == j) R[1] = 0;
    }

    return value;
}

