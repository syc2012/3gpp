#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_rrc_enc(
    uint8  K_RRC_ENC[32], // OUT
    uint8  K_gNB[32],     // IN
    uint8  NEAx           // IN
)
{
    uint8  KEY[32];
    uint8  S[8];
    uint8  buf[32];

    memcpy(KEY, K_gNB, 32);

    /* FC */
    S[0] = 0x69;
    /* P0 */
    S[1] = 0x03; /* N-RRC-enc-alg */
    /* L0 */
    S[2] = 0x00;
    S[3] = 0x01;
    /* P1 */
    S[4] = NEAx; /* 5G-EAx */
    /* L1 */
    S[5] = 0x00;
    S[6] = 0x01;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 7);

    kdf(KEY, 32, S, 7, buf);
    memcpy(K_RRC_ENC, buf+16, 16);
    mem_dump("K_RRC_ENC", K_RRC_ENC, 16);
}

