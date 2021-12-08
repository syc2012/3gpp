#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_nh(
    uint8  K_NH[32],  // OUT
    uint8  K_AMF[32], // IN
    uint8  K_gNB[32]  // IN
)
{
    uint8  KEY[32];
    uint8  S[36];

    memcpy(KEY, K_AMF, 32);

    /* FC */
    S[0]  = 0x6F;
    /* P0 */
    memcpy(S+1, K_gNB, 32);
    /* L0 */
    S[33] = 0x00;
    S[34] = 0x20;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 35);

    kdf(KEY, 32, S, 35, K_NH);
    mem_dump("K_NH", K_NH, 32);
}

