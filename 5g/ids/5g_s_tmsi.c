#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: 5g_s_tmsi [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -s ID     AMF Set ID (10 bits).\n");
    printf("  -t ID     AMF Pointer (6 bits).\n");
    printf("  -5 ID     5G-TMSI (4 bytes).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.3.20 5G-S-TMSI */
int main(int argc, char *argv[])
{
    unsigned char  plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned short amfSetId = 0x201;
    unsigned char  amfPointer = 0x21;
    unsigned int   _5gTmsi = 0x80000001;
    char plmnStr[8];
    char _5gTmsiStr[64];
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "p:s:t:5:h")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 's':
                amfSetId = (atoi( optarg ) & 0x3FF);
                break;
            case 't':
                amfPointer = (atoi( optarg ) & 0x3F);
                break;
            case '5':
                _5gTmsi = (strtoul(optarg, NULL, 10) & 0xFFFFFFFF);
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);
    int2hex(_5gTmsi, 32, _5gTmsiStr);

    printf("5G-S-TMSI\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + AMF Set ID\n");
    printf("    + [1;33m%u[0m (10 bits)\n", amfSetId);
    printf(" + AMF Pointer\n");
    printf("    + [1;33m%u[0m (6 bits)\n", amfPointer);
    printf(" + 5G-TMSI\n");
    printf("    + [1;33m%u / %s[0m (8 bits)\n", _5gTmsi, _5gTmsiStr);

    return 0;
}

