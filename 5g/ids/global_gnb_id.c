#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: global_gnb_id [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -g ID     gNB ID.\n");
    printf("  -b BITS   gNB ID bits (22 ~ 32).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.1.6 Global gNB ID */
int main(int argc, char *argv[])
{
    unsigned char plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned int  gnbId = 0x200001;
    int gnbIdBits = 22;
    char plmnStr[8];
    char gnbIdStr[64];
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "p:g:b:h")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 'g':
                gnbId = strtoul(optarg, NULL, 10);
                break;
            case 'b':
                gnbIdBits = atoi( optarg );
                if (gnbIdBits < 22) gnbIdBits = 22;
                if (gnbIdBits > 32) gnbIdBits = 32;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);
    gnbId &= (32 == gnbIdBits) ? 0xFFFFFFFF : ((0x1 << gnbIdBits) - 1);
    int2hex(gnbId, gnbIdBits, gnbIdStr);

    printf("Global gNB ID\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + gNB ID\n");
    printf("    + [1;33m%u / %s[0m (%d bits)\n", gnbId, gnbIdStr, gnbIdBits);

    return 0;
}

