#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_seaf(
    uint8  K_SEAF[32], // OUT
    uint8  K_AUSF[32], // IN
    uint8  SNN[32]     // IN
)
{
    uint8  KEY[32];
    uint8  S[35];


    memcpy(KEY, K_AUSF, 32);

    /* FC */
    S[0]  = 0x6C;
    /* P0 */
    memcpy(S+1, SNN, 32);
    /* L0 */
    S[33] = 0x00;
    S[34] = 0x20;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 35);

    kdf(KEY, 32, S, 35, K_SEAF);
    mem_dump("K_SEAF", K_SEAF, 32);
}

