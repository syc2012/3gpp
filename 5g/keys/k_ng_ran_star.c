#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_ng_ran_star(
    uint8  K_NG_RAN_STAR[32], // OUT
    uint8  K_gNB[32],         // IN
    uint16 PCI,               // IN
    uint32 ARFCN              // IN
)
{
    uint8  KEY[32];
    uint8  S[10];
    uint8  buf[32];

    memcpy(KEY, K_gNB, 32);

    /* FC */
    S[0] = 0x70;
    /* P0 */
    S[1] = ((PCI >> 8) & 0xFF); /* PCI */
    S[2] = ((PCI     ) & 0xFF);
    /* L0 */
    S[3] = 0x00;
    S[4] = 0x02;
    /* P1 */
    S[5] = ((ARFCN >> 16) & 0xFF); /* ARFCN-DL */
    S[6] = ((ARFCN >>  8) & 0xFF);
    S[7] = ((ARFCN      ) & 0xFF);
    /* L1 */
    S[8] = 0x00;
    S[9] = 0x03;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 10);

    kdf(KEY, 32, S, 10, buf);
    memcpy(K_NG_RAN_STAR, buf, 32);
    mem_dump("K_NG_RAN_STAR", K_NG_RAN_STAR, 32);
}

