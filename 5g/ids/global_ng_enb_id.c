#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: global_ng_enb_id [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -e ID     ng-eNB ID.\n");
    printf("  -m        Macro ng-eNB ID (20-bit).\n");
    printf("  -s        Short Macro ng-eNB ID (18-bit).\n");
    printf("  -l        Long Macro ng-eNB ID (21-bit).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.1.8 Global ng-eNB ID */
int main(int argc, char *argv[])
{
    unsigned char plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned int  ngEnbId = 0x80001;
    int ngEnbIdBits = 20;
    char plmnStr[8];
    char ngEnbIdStr[64];
    char ngEnbType[64];
    int opt;


    sprintf(ngEnbType, "Macro");

    opterr = 0;
    while ((opt=getopt(argc, argv, "p:e:mslh")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 'e':
                ngEnbId = strtoul(optarg, NULL, 10);
                break;
            case 'm':
                ngEnbIdBits = 20;
                sprintf(ngEnbType, "Macro");
                break;
            case 's':
                ngEnbIdBits = 18;
                sprintf(ngEnbType, "Short Macro");
                break;
            case 'l':
                ngEnbIdBits = 21;
                sprintf(ngEnbType, "Long Macro");
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);
    ngEnbId &= ((0x1 << ngEnbIdBits) - 1);
    int2hex(ngEnbId, ngEnbIdBits, ngEnbIdStr);

    printf("Global ng-eNB ID\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + %s ng-eNB ID\n", ngEnbType);
    printf("    + [1;33m%u / %s[0m (%d bits)\n", ngEnbId, ngEnbIdStr, ngEnbIdBits);

    return 0;
}

