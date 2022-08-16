#include "kdf.h"
#include <netinet/in.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#define blk0(i) (block->l[i] = htonl(block->l[i]))
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);



// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/*
* c = (a ^ b)
*/
void xor(uint8 *c, uint8 *a, uint8 *b, int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        c[i] = a[i] ^ b[i];
    }
}

void dump(char *name, const void *addr, int len)
{
    uint8 *p = (uint8 *)addr;
    uint32   i;

    printf("%s\n", name);

    if (p == NULL)
    {
        printf("Null\n");
        return;
    }

    for (i=0; i<len; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf("%02X ", p[i]);
    }
    printf("\n\n");
}


void nossl_SHA1_Transform(unsigned int state[5], const unsigned char buffer[64])
{
    unsigned int a, b, c, d, e;
    typedef union {
        unsigned char c[64];
        unsigned int  l[16];
    } CHAR64LONG16;
    CHAR64LONG16 *block;

    block = (CHAR64LONG16 *) buffer;

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;
}

/* Pseudo random number generator
 * FIPS 186-2 Appendix 3.1 with change notes: Random Number Generation: 1
 * Uses the SHA1 transform function as G().
 *
 * Based on the initial value XKEY a set of pseudo-random numbers are
 * generated. The generated numbers are placed in x, which must be allocated
 * prior to calling fips186_2_prng(). On error x is reset to all zero;
 *
 * Currently the implementation has the following restrictions:
 *   XKEY must be 20 bytes long (160 bit).
 *   XSEEDj is not supported.
 *   x must be multiple of 40 bytes (320 bit).
 */
void fips186_2_prng(
    uint8 *XKEY,
    int    XKEYlen,
    uint8 *XSEEDj,
    int    XSEEDjlen,
    uint8 *x,
    int    xlen
)
{
    uint32 addresult, carry;
    uint32 state[5] = {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
        0xc3d2e1f0
    };
    uint32 t[5];
    uint8 xval[64];
    uint8 w[20];
    int i, j, m;
    int n;

    /* XSEEDj is not (yet) supported */
    if (XSEEDjlen != 0)
    {
        memset(x, 0, xlen);
        return;
    }

    /* XKEYlen must be 20 */
    if (XKEYlen != 20)
    {
        memset(x, 0, xlen);
        return;
    }

    /* xlen must be multiple of 40 */
    if (xlen % 40)
    {
        memset(x, 0, xlen);
        return;
    }
    m = xlen / 40;  /* Number of iterations */

    /* Step 3 */
    for (j=0; j<=(m-1); j++)
    {
        /* Step 3.2 */
        for (i=0; i<=1; i++)
        {
            /* a. XVAL = (XKEY + XSEED_j) mod 2^b */
            for (n=0;n<20;n++)  xval[n] = XKEY[n]; /* FIPS 186-2 Appendix 3.3 */
            for (n=20;n<64;n++) xval[n] = 0;

            /* b. w_i = G(t, XVAL) */
            memcpy(t, state, 20);
            nossl_SHA1_Transform(t, xval);
            t[0] = htonl(t[0]);
            t[1] = htonl(t[1]);
            t[2] = htonl(t[2]);
            t[3] = htonl(t[3]);
            t[4] = htonl(t[4]);
            memcpy(w, t, 20);

            /* c. XKEY = (1 + XKEY + w_i) mod 2^b */
            carry = 0;
            for (n=19; n>=0; n--)
            {
                if (n==19) addresult = 1; else addresult = 0;
                addresult += XKEY[n] + w[n] + carry;
                XKEY[n] = (addresult & 0x00ff);
                carry = ((addresult & 0xff00) >> 8);
            }

            /* Step 3.3 xj = (w0 | w1) mod q */
            memcpy(&x[j*40+i*20], w, XKEYlen);
        }
    }
}


/*
* 33.402 A.2 Function for the derivation of CK', IK' from CK, IK
*/
uint32 kdf_ck_ik_prime(
    uint8  *CK,
    uint8  *IK,
    char   *SNN,
    uint8  *AUTN,
    uint8  *out,
    uint32  outLen
)
{
    uint8   key[32];
    uint8   in[256];
    uint32  inLen = 0;

    memcpy((key     ), CK, 16);
    memcpy((key + 16), IK, 16);

    /* FC (0x20) */
    in[0] = 0x20;
    inLen += 1;

    /* P0 (network identity) */
    memcpy((in + inLen), SNN, strlen(SNN));
    inLen += strlen(SNN);

    /* L0 */
    UINT16_TO_BYTE_ARRAY(strlen(SNN), (in + inLen));
    inLen += 2;

    /* P1 (SQN^AK) */
    memcpy((in + inLen), AUTN, 6);
    inLen += 6;

    /* L1 */
    UINT16_TO_BYTE_ARRAY(6, (in + inLen));
    inLen += 2;

    HMAC(EVP_sha256(), key, 32, in, inLen, out, &outLen);

    return outLen;
}

/*
* EAP-AKA' pseudo-random function
*/
void PRF_prime(
    uint8  *key,
    char   *string,
    uint8  *out,
    uint32  outLen
)
{
    uint8  *mac[2] = { out, out };
    uint8   in[256];
    uint32  inLen = 0;
    uint32  macLen = 32;
    int  loop;
    int  i;


    loop = (outLen / 32);
    if ((outLen % 32) > 0)
    {
        loop++;
    }

    outLen = 0;
    for (i=0; i<loop; i++)
    {
        /*
        * PRF'(K, S) = T1 | T2 | T3 | T4 | ...
        *
        * T1 = HMAC-SHA-256(K, (S | 0x01))
        * T2 = HMAC-SHA-256(K, (T1 | S | 0x02))
        * T3 = HMAC-SHA-256(K, (T2 | S | 0x03))
        * T4 = HMAC-SHA-256(K, (T3 | S | 0x04))
        * ...
        */

        inLen = 0;
        if (i > 0)
        {
            memcpy(in, mac[0], 32);
            inLen += 32;
        }
        memcpy((in + inLen), string, strlen(string));
        inLen += strlen(string);
        in[inLen] = (i + 1);
        inLen += 1;

        HMAC(EVP_sha256(), key, 32, in, inLen, mac[1], &macLen);
        outLen += 32;
        mac[0] = mac[1];
        mac[1] += 32;
    }
}

