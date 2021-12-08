#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


void k_ausf(
    uint8  K_AUSF[32], // OUT
    uint8  CK[16],     // IN
    uint8  IK[16],     // IN
    uint8  AK[6],      // IN
    uint8  SQN[6],     // IN
    uint8  SNN[32]     // IN
)
{
    uint8  KEY[32];
    uint8  S[43];

    memcpy(KEY,    CK, 16);
    memcpy(KEY+16, IK, 16);

    /* FC */
    S[0]  = 0x6A;
    /* P0 */
    memcpy(S+1, SNN, 32);
    /* L0 */
    S[33] = 0x00;
    S[34] = 0x20;
    /* P1 */
    S[35] = (SQN[0] ^ AK[0]);
    S[36] = (SQN[1] ^ AK[1]);
    S[37] = (SQN[2] ^ AK[2]);
    S[38] = (SQN[3] ^ AK[3]);
    S[39] = (SQN[4] ^ AK[4]);
    S[40] = (SQN[5] ^ AK[5]);
    /* L1 */
    S[41] = 0x00;
    S[42] = 0x06;

    //mem_dump("Key", KEY, 32);
    //mem_dump("S", S, 43);

    kdf(KEY, 32, S, 43, K_AUSF);
    mem_dump("K_AUSF", K_AUSF, 32);
}

