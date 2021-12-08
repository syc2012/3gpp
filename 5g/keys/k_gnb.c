#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_gnb(
    uint8  K_gNB[32], // OUT
    uint8  K_AMF[32], // IN
    uint32 NAS_COUNT  // IN
)
{
    uint8  KEY[32];
    uint8  S[10];

    memcpy(KEY, K_AMF, 32);

    /* FC */
    S[0] = 0x6E;
    /* P0 */
    S[1]  = ((NAS_COUNT >> 24) & 0xFF);
    S[2]  = ((NAS_COUNT >> 16) & 0xFF);
    S[3]  = ((NAS_COUNT >>  8) & 0xFF);
    S[4]  = ((NAS_COUNT      ) & 0xFF);
    /* L0 */
    S[5] = 0x00;
    S[6] = 0x04;
    /* P1 */
    S[7] = 0x01; /* 3GPP access */
    /* L1 */
    S[8] = 0x00;
    S[9] = 0x01;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 10);

    kdf(KEY, 32, S, 10, K_gNB);
    mem_dump("K_gNB", K_gNB, 32);
}

