#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


void help(void)
{
    printf("Usage: nr_cgi [OPTION]...\n");
    printf("\n");
    printf("  -p ID     PLMN ID (MCC.MNC).\n");
    printf("  -n ID     NR Cell ID (36 bits).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/* 38.413 Section 9.3.1.7 NR CGI */
int main(int argc, char *argv[])
{
    unsigned char plmnId[3] = { 0x21, 0xF3, 0x54 };
    unsigned long long nrCellId = 0x800000001LL;
    char plmnStr[8];
    char nrCellStr[64];
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "p:n:h")) != -1)
    {
        switch ( opt )
        {
            case 'p':
                str2plmn(optarg, plmnId);
                break;
            case 'n':
                nrCellId = strtoull(optarg, NULL, 10);
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    plmn2str(plmnId, plmnStr);
    nrCellId &= ((0x1LL << 36) - 1);
    int2hex(nrCellId, 36, nrCellStr);

    printf("NR CGI\n");
    printf(" + PLMN ID\n");
    printf("    + [1;33m%s / %02X %02X %02X[0m\n", plmnStr, plmnId[0], plmnId[1], plmnId[2]);
    printf(" + NR Cell ID\n");
    printf("    + [1;33m%llu / %s[0m (36 bits)\n", nrCellId, nrCellStr);

    return 0;
}

