#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void xres_star(
    uint8  XRES_STAR[16], // OUT
    uint8  XRES[16],      // IN
    int    XRES_LEN,      // IN
    uint8  _RAND[16],     // IN
    uint8  CK[16],        // IN
    uint8  IK[16],        // IN
    uint8  SNN[32]        // IN
)
{
    uint8  KEY[32];
    uint8  S[71];
    uint8  buf[32];

    memcpy(KEY,    CK, 16);
    memcpy(KEY+16, IK, 16);

    if (8 == XRES_LEN) /* 8-byte XRES */
    {
        /* FC */
        S[0]  = 0x6B;
        /* P0 */
        memcpy(S+1, SNN, 32);
        /* L0 */
        S[33] = 0x00;
        S[34] = 0x20;
        /* P1 */
        memcpy(S+35, _RAND, 16);
        /* L1 */
        S[51] = 0x00;
        S[52] = 0x10;
        /* P2 */
        memcpy(S+53, XRES, 8);
        /* L2 */
        S[61] = 0x00;
        S[62] = 0x08;

        //mem_dump("Key", KEY, 32);
        //mem_dump("S", S, 63);

        kdf(KEY, 32, S, 63, buf);
    }
    else /* 16-byte XRES */
    {
        /* FC */
        S[0]  = 0x6B;
        /* P0 */
        memcpy(S+1, SNN, 32);
        /* L0 */
        S[33] = 0x00;
        S[34] = 0x20;
        /* P1 */
        memcpy(S+35, _RAND, 16);
        /* L1 */
        S[51] = 0x00;
        S[52] = 0x10;
        /* P2 */
        memcpy(S+53, XRES, 16);
        /* L2 */
        S[69] = 0x00;
        S[70] = 0x10;

        //mem_dump("Key", KEY, 32);
        //mem_dump("S", S, 71);

        kdf(KEY, 32, S, 71, buf);
    }

    memcpy(XRES_STAR, buf+16, 16);
    mem_dump("XRES*", XRES_STAR, 16);
}

