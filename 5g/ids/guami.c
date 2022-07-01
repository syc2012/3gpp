#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: guami [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -r ID     AMF Region ID (8 bits).\n");
    printf("  -s ID     AMF Set ID (10 bits).\n");
    printf("  -t ID     AMF Pointer (6 bits).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.3.3 GUAMI */
int main(int argc, char *argv[])
{
    unsigned char  plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned char  amfRegionId = 0x81;
    unsigned short amfSetId = 0x201;
    unsigned char  amfPointer = 0x21;
    char plmnStr[8];
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "p:r:s:t:h")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 'r':
                amfRegionId = (atoi( optarg ) & 0xFF);
                break;
            case 's':
                amfSetId = (atoi( optarg ) & 0x3FF);
                break;
            case 't':
                amfPointer = (atoi( optarg ) & 0x3F);
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);

    printf("GUAMI\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + AMF Region ID\n");
    printf("    + [1;33m%u[0m (8 bits)\n", amfRegionId);
    printf(" + AMF Set ID\n");
    printf("    + [1;33m%u[0m (10 bits)\n", amfSetId);
    printf(" + AMF Pointer\n");
    printf("    + [1;33m%u[0m (6 bits)\n", amfPointer);

    return 0;
}

