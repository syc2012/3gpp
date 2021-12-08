#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdf.h"


typedef enum
{
    ID_NIA0     = 0,
    ID_128_NIA1 = 1,
    ID_128_NIA2 = 2,
    ID_128_NIA3 = 3
} tNIAx;

typedef enum
{
    ID_NEA0     = 0,
    ID_128_NEA1 = 1,
    ID_128_NEA2 = 2,
    ID_128_NEA3 = 3
} tNEAx;


uint8  _RAND[16] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8  XRES[16] = {
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

uint8  CK[16] = {
 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00
};

uint8  IK[16] = {
 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x01
};

uint8  AK[6] = {
 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

uint8  SQN[6] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
};

uint8  SNN[32] = {
 /* PLMN-ID: 001.01 */
 '5', 'G',
 ':',
 'm', 'n', 'c', '0', '0', '1',
 '.',
 'm', 'c', 'c', '0', '0', '1',
 '.',
 '3', 'g', 'p', 'p', 'n', 'e', 't', 'w', 'o', 'r', 'k',
 '.',
 'o', 'r', 'g'
};

uint8  SUPI[15] = {
 /* IMSI: 001.01.0123456789 */
 '0', '0', '1',
 '0', '1',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

uint8  ABBA[2] = {
 0x00, 0x00
};

uint32 NAS_COUNT = 0;

int NIAx = ID_128_NIA2;
int NEAx = ID_128_NEA2;

uint16 PCI = 0;
uint32 ARFCN = 630400;


extern void xres_star(
    uint8  XRES_STAR[16], // OUT
    uint8  XRES[16],      // IN
    int    XRES_LEN,      // IN
    uint8  _RAND[16],     // IN
    uint8  CK[16],        // IN
    uint8  IK[16],        // IN
    uint8  SNN[32]        // IN
);

extern void hxres_star(
    uint8  HXRES_STAR[16], // OUT
    uint8  XRES_STAR[16],  // IN
    uint8  _RAND[16]       // IN
);

extern void k_ausf(
    uint8  K_AUSF[32], // OUT
    uint8  CK[16],     // IN
    uint8  IK[16],     // IN
    uint8  AK[6],      // IN
    uint8  SQN[6],     // IN
    uint8  SNN[32]     // IN
);

extern void k_seaf(
    uint8  K_SEAF[32], // OUT
    uint8  K_AUSF[32], // IN
    uint8  SNN[32]     // IN
);

extern void k_amf(
    uint8  K_AMF[32],  // OUT
    uint8  K_SEAF[32], // IN
    uint8  SUPI[15],   // IN
    uint8  ABBA[2]     // IN
);

extern void k_nas_int(
    uint8  K_NAS_INT[16], // OUT
    uint8  K_AMF[32],     // IN
    int    NIAx           // IN
);

extern void k_nas_enc(
    uint8  K_NAS_ENC[16], // OUT
    uint8  K_AMF[32],     // IN
    int    NEAx           // IN
);

extern void k_gnb(
    uint8  K_gNB[32], // OUT
    uint8  K_AMF[32], // IN
    uint32 NAS_COUNT  // IN
);

extern void k_nh(
    uint8  K_NH[32],  // OUT
    uint8  K_AMF[32], // IN
    uint8  K_gNB[32]  // IN
);

extern void k_rrc_int(
    uint8  K_RRC_INT[32], // OUT
    uint8  K_gNB[32],     // IN
    uint8  NIAx           // IN
);

extern void k_rrc_enc(
    uint8  K_RRC_ENC[32], // OUT
    uint8  K_gNB[32],     // IN
    uint8  NEAx           // IN
);

extern void k_up_int(
    uint8  K_UP_INT[16], // OUT
    uint8  K_gNB[32],    // IN
    uint8  NIAx          // IN
);

extern void k_up_enc(
    uint8  K_UP_ENC[16], // OUT
    uint8  K_gNB[32],    // IN
    uint8  NEAx          // IN
);

extern void k_ng_ran_star(
    uint8  K_NG_RAN_STAR[32], // OUT
    uint8  K_gNB[32],         // IN
    uint16 PCI,               // IN
    uint32 ARFCN              // IN
);

int main(void)
{
    uint8  XRES_STAR[16];
    uint8  HXRES_STAR[16];
    uint8  K_AUSF[32];
    uint8  K_SEAF[32];
    uint8  K_AMF[32];
    uint8  K_NAS_ENC[16];
    uint8  K_NAS_INT[16];
    uint8  K_gNB[32];
    uint8  K_NH[32];
    uint8  K_RRC_ENC[16];
    uint8  K_RRC_INT[16];
    uint8  K_UP_ENC[16];
    uint8  K_UP_INT[16];
    uint8  K_NG_RAN_STAR[32];


    xres_star(XRES_STAR, XRES, 16, _RAND, CK, IK, SNN);

    hxres_star(HXRES_STAR, XRES_STAR, _RAND);

    k_ausf(K_AUSF, CK, IK, AK, SQN, SNN);

    k_seaf(K_SEAF, K_AUSF, SNN);

    k_amf(K_AMF, K_SEAF, SUPI, ABBA);

    k_nas_enc(K_NAS_ENC, K_AMF, NEAx);

    k_nas_int(K_NAS_INT, K_AMF, NIAx);

    k_gnb(K_gNB, K_AMF, NAS_COUNT);

    k_nh(K_NH, K_AMF, K_gNB);

    k_rrc_enc(K_RRC_ENC, K_gNB, NEAx);

    k_rrc_int(K_RRC_INT, K_gNB, NIAx);

    k_up_enc(K_UP_ENC, K_gNB, NEAx);

    k_up_int(K_UP_INT, K_gNB, NIAx);

    k_ng_ran_star(K_NG_RAN_STAR, K_gNB, PCI, ARFCN);


    return 0;
}

