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

char  Identity[] = "0001016000000003";

/* Peer <- Authenticator */
uint8 ChallengeReq[68] = {
 /* EAP Request */
 0x01, 0x01, 0x00, 0x44,
 /* EAP-AKA Challenge */
 0x17, 0x01, 0x00, 0x00,
 /* AT_RAND */
 0x01, 0x05, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 /* AT_AUTN */
 0x02, 0x05, 0x00, 0x00,
 0x03, 0x04, 0x05, 0x06, 0x07, 0xF7, 0x90, 0x01,
 0x00, 0x01, 0x02, 0x03, 0x04, 0xFA, 0x96, 0x06,
 /* AT_MAC */
 0x0b, 0x05, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Peer -> Authenticator */
uint8 ChallengeRsp[40] = {
 /* EAP Response */
 0x02, 0x01, 0x00, 0x28,
 /* EAP-AKA Challenge */
 0x17, 0x01, 0x00, 0x00,
 /* AT_RES */
 0x03, 0x03, 0x00, 0x08,
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 /* AT_MAC */
 0x0b, 0x05, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AT_MAC[2][16] = {
 {
  /* EAP-Request/AKA/Challenge */
  0x8E, 0x8E, 0x77, 0x57, 0xE9, 0x69, 0xAB, 0x87,
  0x22, 0x7C, 0x03, 0xBB, 0xA0, 0xED, 0xF4, 0x31
 },
 {
  /* EAP-Response/AKA/Challenge */
  0x21, 0x82, 0x57, 0x50, 0x03, 0x3D, 0x11, 0x38,
  0x56, 0xD8, 0xBB, 0xDA, 0x13, 0xA5, 0x20, 0xAD
 }
};

/*
* AKA algorithm:
*  MAC = f1(K, AMF, SQN, RAND)
*  RES = f2(K, RAND)
*  CK  = f3(K, RAND)
*  IK  = f4(K, RAND)
*  AK  = f5(K, RAND)
*  AUTN = (SQN ^ AK) || AMF || MAC
*/
uint8 CK[16] = {
 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00
};
uint8 IK[16] = {
 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01
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
    uint8  data[160];
    uint8 *pBuf = data;


    memcpy(pBuf, Identity, strlen(Identity));
    pBuf += strlen(Identity);
    memcpy(pBuf, IK, 16);
    pBuf += 16;
    memcpy(pBuf, CK, 16);
    pBuf += 16;

    /*
    * MK = SHA1(Identity || IK || CK)
    */
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
    memset(data, 0, 160);
    fips186_2_prng(MK, 20, NULL, 0, data, 160);
    memcpy(K_encr, data,      16);
    memcpy(K_aut,  data + 16, 16);
    memcpy(MSK,    data + 32, 64);
    memcpy(EMSK,   data + 96, 64);

    dump("[K_encr]", K_encr, 16);
    dump("[K_aut]", K_aut, 16);
    dump("[MSK]", MSK, 64);
    dump("[EMSK]", EMSK, 64);


    /* 
    * EAP-Request/AKA/Challenge
    *  AT_MAC = HMAC-SHA1-128(EAP packet, K_aut)
    */
    memset(MAC, 0, MAC_len);
    HMAC(EVP_sha1(), K_aut, 16, ChallengeReq, 68, MAC, &MAC_len);

    dump("[MAC] Challenge Request", MAC, 16);
    if (0 != memcmp(&(AT_MAC[0][0]), MAC, 16))
    {
        printf("ERR: incorrect MAC\n");
        dump("[AT_MAC]", &(AT_MAC[0][0]), 16);
    }

    /*
    * EAP-Response/AKA/Challenge
    *  AT_MAC = HMAC-SHA1-128(EAP packet, K_aut)
    */
    memset(MAC, 0, MAC_len);
    HMAC(EVP_sha1(), K_aut, 16, ChallengeRsp, 40, MAC, &MAC_len);

    dump("[MAC] Challenge Response", MAC, 16);
    if (0 != memcmp(&(AT_MAC[1][0]), MAC, 16))
    {
        printf("ERR: incorrect MAC\n");
        dump("[AT_MAC]", &(AT_MAC[1][0]), 16);
    }

    return 0;
}

