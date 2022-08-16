#include "kdf.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

/* RFC 4186 Appendix A. Test Vectors */
char  Identity[] = "1244070100000001@eapsim.foo";
uint8 Nonce[16] = {
 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
};
uint8 VersionList[2] = { 0x00, 0x01 };
uint8 SelectedVersion[2] = { 0x00, 0x01 };

/* Peer <- Authenticator */
uint8 ChallengeReq[80] = {
 /* EAP Request */
 0x01, 0x02, 0x00, 0x50,
 /* EAP-SIM Challenge */
 0x12, 0x0b, 0x00, 0x00,
 /* AT_RAND */
 0x01, 0x0d, 0x00, 0x00,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
 /* AT_MAC */
 0x0b, 0x05, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Peer -> Authenticator */
uint8 ChallengeRsp[28] = {
 /* EAP Response */
 0x02, 0x02, 0x00, 0x1c,
 /* EAP-SIM Challenge */
 0x12, 0x0b, 0x00, 0x00,
 /* AT_MAC */
 0x0b, 0x05, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AT_MAC[2][16] = {
 {
  /* EAP-Request/SIM/Challenge */
  0x63, 0x70, 0x9C, 0x98, 0xE2, 0x32, 0x27, 0xD0,
  0x81, 0x14, 0xFA, 0xC0, 0xB4, 0x95, 0xD9, 0xBE
 },
 {
  /* EAP-Response/SIM/Challenge */
  0xf5, 0x6d, 0x64, 0x33, 0xe6, 0x8e, 0xd2, 0x97,
  0x6a, 0xc1, 0x19, 0x37, 0xfc, 0x3d, 0x11, 0x54
 }
};


/*
* GSM algorithm:
*  SRES, Kc = A3A8(RAND, Ki)
*/
uint8 SRES[3][4] = {
 { 0xd1, 0xd2, 0xd3, 0xd4 },
 { 0xe1, 0xe2, 0xe3, 0xe4 },
 { 0xf1, 0xf2, 0xf3, 0xf4 }
};

uint8 Kc[3][8] = {
 { 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7 },
 { 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7 },
 { 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7 }
};


uint8  MK[20];
uint8  K_encr[16];
uint8  K_aut[16];
uint8  MSK[64];
uint8  EMSK[64];
uint8  MAC[20];
uint32 MAC_len = 20;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    uint8  data[256];
    uint8 *pBuf = data;
    int  i;


    /*
    * MK = SHA1(
    *       Identity ||
    *       n*Kc ||
    *       NONCE_MT ||
    *       Version List ||
    *       Selected Version
    *      )
    */
    memset(pBuf, 0, 256);
    memcpy(pBuf, Identity, strlen(Identity));
    pBuf += strlen(Identity);
    for (i=0; i<3; i++)
    {
        memcpy(pBuf, &(Kc[i][0]), 8);
        pBuf += 8;
    }
    memcpy(pBuf, Nonce, 16);
    pBuf += 16;
    memcpy(pBuf, VersionList, 2);
    pBuf += 2;
    memcpy(pBuf, SelectedVersion, 2);
    pBuf += 2;

    SHA1(data, (pBuf - data), MK);
    dump("[MK]", MK, 20);


    /*
    * X[160] = PRF( MK )
    *
    * K_encr = X[ 0: 15]
    * K_aut  = X[16: 31]
    * MSK    = X[32: 95]
    * EMSK   = X[96:159]
    */
    pBuf = data;
    memset(pBuf, 0, 256);
    fips186_2_prng(MK, 20, NULL, 0, pBuf, 160);

    memcpy(K_encr, pBuf, 16);
    pBuf += 16;
    dump("[K_encr]", K_encr, 16);

    memcpy(K_aut, pBuf, 16);
    pBuf += 16;
    dump("[K_aut]", K_aut, 16);

    memcpy(MSK, pBuf, 64);
    pBuf += 64;
    dump("[MSK]", MSK, 64);

    memcpy(EMSK, pBuf, 64);
    pBuf += 64;
    dump("[EMSK]", EMSK, 64);


    /* 
    * EAP-Request/SIM/Challenge
    *  AT_MAC = HMAC-SHA1-128((EAP packet || NONCE_MT), K_aut)
    */
    pBuf = data;
    memset(pBuf, 0, 256);
    memcpy(pBuf, ChallengeReq, 80);
    pBuf += 80;
    memcpy(pBuf, Nonce, 16);
    pBuf += 16;

    memset(MAC, 0, MAC_len);
    HMAC(EVP_sha1(), K_aut, 16, data, (80 + 16), MAC, &MAC_len);

    dump("[MAC] Challenge Request", MAC, 16);
    if (0 != memcmp(&(AT_MAC[0][0]), MAC, 16))
    {
        printf("ERR: incorrect MAC\n");
        dump("[AT_MAC]", &(AT_MAC[0][0]), 16);
    }

    /*
    * EAP-Response/SIM/Challenge
    *  AT_MAC = HMAC-SHA1-128((EAP packet || n*SRES), K_aut)
    */
    pBuf = data;
    memset(pBuf, 0, 256);
    memcpy(pBuf, ChallengeRsp, 28);
    pBuf += 28;
    for (i=0; i<3; i++)
    {
        memcpy(pBuf, &(SRES[i][0]), 4);
        pBuf += 4;
    }

    memset(MAC, 0, MAC_len);
    HMAC(EVP_sha1(), K_aut, 16, data, (28 + 12), MAC, &MAC_len);

    dump("[MAC] Challenge Response", MAC, 16);
    if (0 != memcmp(&(AT_MAC[1][0]), MAC, 16))
    {
        printf("ERR: incorrect MAC\n");
        dump("[AT_MAC]", &(AT_MAC[1][0]), 16);
    }

    return 0;
}

