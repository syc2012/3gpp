#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: tai [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -t ID     TAC (3 bytes).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.3.11 TAI */
int main(int argc, char *argv[])
{
    unsigned char plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned int  tac = 0x112233;
    char plmnStr[8];
    char tacStr[64];
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "p:t:h")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 't':
                tac = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);
    tac &= 0xFFFFFF;
    int2hex(tac, 24, tacStr);

    printf("NR CGI\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + TAC\n");
    printf("    + [1;33m%u / %s[0m (3 bytes)\n", tac, tacStr);

    return 0;
}

