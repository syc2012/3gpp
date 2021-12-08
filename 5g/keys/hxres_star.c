#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "kdf.h"


void hxres_star(
    uint8  HXRES_STAR[16], // OUT
    uint8  XRES_STAR[16],  // IN
    uint8  _RAND[16]       // IN
)
{
    uint8  S[32];
    uint8  buf[32];

    /* P0 */
    memcpy(S, _RAND, 16);
    /* P1 */
    memcpy(S+16, XRES_STAR, 16);

    //mem_dump("S", S, 32);

    SHA256(S, 32, buf);
    memcpy(HXRES_STAR, buf+16, 16);
    mem_dump("HXRES*", HXRES_STAR, 16);
}

