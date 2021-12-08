#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_amf(
    uint8  K_AMF[32],  // OUT
    uint8  K_SEAF[32], // IN
    uint8  SUPI[15],   // IN
    uint8  ABBA[2]     // IN
)
{
    uint8  KEY[32];
    uint8  S[22];

    memcpy(KEY, K_SEAF, 32);

    /* FC */
    S[0]  = 0x6D;
    /* P0 */
    memcpy(S+1, SUPI, 15);
    /* L0 */
    S[16] = 0x00;
    S[17] = 0x0F;
    /* P1 */
    S[18] = ABBA[0];
    S[19] = ABBA[1];
    /* L1 */
    S[20] = 0x00;
    S[21] = 0x02;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 22);

    kdf(KEY, 32, S, 22, K_AMF);
    mem_dump("K_AMF", K_AMF, 32);
}

