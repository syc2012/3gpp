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

/* RFC 5448 Appendix C. Test Vectors */
char  Identity[] = "0555444333222111";
char  NetworkName[] = "WLAN";

/*
* AKA algorithm:
*  MAC = f1(K, AMF, SQN, RAND)
*  RES = f2(K, RAND)
*  CK  = f3(K, RAND)
*  IK  = f4(K, RAND)
*  AK  = f5(K, RAND)
*  AUTN = (SQN ^ AK) || AMF || MAC
*/
uint8 RAND[16] = {
 0x81, 0xe9, 0x2b, 0x6c, 0x0e, 0xe0, 0xe1, 0x2e,
 0xbc, 0xeb, 0xa8, 0xd9, 0x2a, 0x99, 0xdf, 0xa5
};
uint8 AUTN[16] = {
 0xbb, 0x52, 0xe9, 0x1c, 0x74, 0x7a, 0xc3, 0xab,
 0x2a, 0x5c, 0x23, 0xd1, 0x5e, 0xe3, 0x51, 0xd5
};
uint8 IK[16] = {
 0x97, 0x44, 0x87, 0x1a, 0xd3, 0x2b, 0xf9, 0xbb,
 0xd1, 0xdd, 0x5c, 0xe5, 0x4e, 0x3e, 0x2e, 0x5a
};
uint8 CK[16] = {
 0x53, 0x49, 0xfb, 0xe0, 0x98, 0x64, 0x9f, 0x94,
 0x8f, 0x5d, 0x2e, 0x97, 0x3a, 0x81, 0xc0, 0x0f
};
uint8 RES[8] = {
 0x28, 0xd7, 0xb0, 0xf2, 0xa2, 0xec, 0x3d, 0xe5
};


uint8  CK_P[16];
uint8  IK_P[16];
uint8  MK[208];
uint8  K_encr[16];
uint8  K_aut[32];
uint8  K_re[32];
uint8  MSK[64];
uint8  EMSK[64];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    uint8 buf[128];
    uint8 ckik[32];


    /*
    * CK', IK' = KDF(CK, IK, SNN, AUTN)
    */
    kdf_ck_ik_prime(
        CK,
        IK,
        NetworkName,
        AUTN,
        ckik,
        32
    );

    dump("[CK']", ckik,      16);
    dump("[IK']", ckik + 16, 16);


    xor(buf, ckik, (ckik + 16), 16);
    xor(ckik, buf, ckik, 16);
    xor((ckik + 16), buf, (ckik + 16), 16);

    sprintf((char *)buf, "EAP-AKA'%s", Identity);

    /*
    * MK = PRF'(IK' || CK', "EAP-AKA'" || Identity)
    * K_encr = MK[  0: 15]
    * K_aut  = MK[ 16: 47]
    * K_re   = MK[ 48: 79]
    * MSK    = MK[ 80:143]
    * EMSK   = MK[144:207]
    */
    PRF_prime(ckik, (char *)buf, MK, 208);
    memcpy(K_encr, MK,       16);
    memcpy(K_aut,  MK +  16, 32);
    memcpy(K_re,   MK +  48, 32);
    memcpy(MSK,    MK +  80, 64);
    memcpy(EMSK,   MK + 144, 64);

    dump("[K_encr]", K_encr, 16);
    dump("[K_aut]", K_aut, 32);
    dump("[K_re]", K_re, 32);
    dump("[MSK]", MSK, 64);
    dump("[EMSK]", EMSK, 64);


    return 0;
}

