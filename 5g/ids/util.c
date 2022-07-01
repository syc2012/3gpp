#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void str2plmn(char *pStr, unsigned char *pBuf)
{
    if (6 == strlen(pStr))
    {
        pBuf[0] = ((pStr[0] - '0') & 0xF) |
                  ((pStr[1] - '0') <<  4);
        pBuf[1] = ((pStr[2] - '0') & 0xF) | (0xF0);
        pBuf[2] = ((pStr[4] - '0') & 0xF) |
                  ((pStr[5] - '0') <<  4);
    }
    else if (7 == strlen(pStr))
    {
        pBuf[0] = ((pStr[0] - '0') & 0xF) |
                  ((pStr[1] - '0') <<  4);
        pBuf[1] = ((pStr[2] - '0') & 0xF) |
                  ((pStr[4] - '0') <<  4);
        pBuf[2] = ((pStr[5] - '0') & 0xF) |
                  ((pStr[6] - '0') <<  4);
    }
    else
    {
        printf("ERR: incorrect PLMN ID\n");
        memset(pBuf, 0, 3);
    }
}

void plmn2str(unsigned char *pBuf, char *pStr)
{
    if (0xF == (pBuf[1] >> 4))
    {
        sprintf(
            pStr,
            "%u%u%u.%u%u",
            (pBuf[0] & 0xF),
            (pBuf[0] >>  4),
            (pBuf[1] & 0xF),
            (pBuf[2] & 0xF),
            (pBuf[2] >>  4)
        );
    }
    else
    {
        sprintf(
            pStr,
            "%u%u%u.%u%u%u",
            (pBuf[0] & 0xF),
            (pBuf[0] >>  4),
            (pBuf[1] & 0xF),
            (pBuf[1] >>  4),
            (pBuf[2] & 0xF),
            (pBuf[2] >>  4)
        );
    }
}

void int2hex(unsigned long long val, int bits, char *pStr)
{
#define DIV_CEIL(x, y) (((x) + ((y) - 1)) / (y))
    char buf[64];
    int len;
    int i;
    int j;

    val <<= (64 - bits);
    sprintf(buf, "%016llx", val);
    len = DIV_CEIL(bits, 4);

    for (i=0, j=0; i<len; i++)
    {
        pStr[j++] = buf[i];
        if ((i & 0x1) && (i != (len-1))) pStr[j++] = 0x20;
    }
    pStr[j] = 0x00;
}

