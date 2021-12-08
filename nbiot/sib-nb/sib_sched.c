#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sib_common.h"


#define RESERVED_SUBFRAME(frm, subFrm) \
    ((0 == subFrm) || \
     (5 == subFrm) || \
     ((0 == (frm & 0x1)) && (9 == subFrm)))


void help(void)
{
    printf("Usage: sib_sched [OPTION]...\n");
    printf("\n");
    printf("  -c cellId       Cell ID.\n");
    printf("  -s sib1Sched    SIB1-NB scheduling info (0 ~ 11).\n");
    printf("  -p period       SIB2-NB periodicity (64, 128, 256, 512, 1024, 2048, 4096).\n");
    printf("  -r repetition   SIB2-NB repetition (2, 4, 8, 16).\n");
    printf("  -t tbSize       SIB2-NB TB bits (56, 120, 208, 256, 328, 440, 552, 680).\n");
    printf("  -w window       SI window length (160, 320, 480, 640, 960, 1280, 1600).\n");
    printf("  -h              Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.331 Section 5.2.1.2a Scheduling for NB-IoT
*/
int main(int argc, char *argv[])
{
    int cellId = 0;
    int sib1Sched = 0;
    int period = 128;
    int repetition = 4;
    int tbSize = 208;
    int window = 160;

    int n_abs_sf;
    int ch;
    int i;
    int j;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:s:p:r:t:w:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                cellId = atoi( optarg );
                break;
            case 's':
                sib1Sched = atoi( optarg );
                break;
            case 'p':
                period = atoi( optarg );
                break;
            case 'r':
                repetition = atoi( optarg );
                break;
            case 't':
                tbSize = atoi( optarg );
                break;
            case 'w':
                window = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((sib1Sched < 0) || (sib1Sched > 11))
    {
        printf("ERR: incorrect sib1Sched %d\n\n", sib1Sched);
        return -1;
    }
    if ((period != 64) &&
        (period != 128) &&
        (period != 256) &&
        (period != 512) &&
        (period != 1024) &&
        (period != 2048) &&
        (period != 4096))
    {
        printf("ERR: incorrect period %d\n\n", period);
        return -1;
    }
    if ((repetition != 2) &&
        (repetition != 4) &&
        (repetition != 8) &&
        (repetition != 16))
    {
        printf("ERR: incorrect repetition %d\n\n", repetition);
        return -1;
    }
    if ((tbSize != 56) &&
        (tbSize != 120) &&
        (tbSize != 208) &&
        (tbSize != 256) &&
        (tbSize != 328) &&
        (tbSize != 440) &&
        (tbSize != 552) &&
        (tbSize != 680))
    {
        printf("ERR: incorrect tbSize %d\n\n", tbSize);
        return -1;
    }
    if ((window != 160) &&
        (window != 320) &&
        (window != 480) &&
        (window != 640) &&
        (window != 960) &&
        (window != 1280) &&
        (window != 1600))
    {
        printf("ERR: incorrect window %d\n\n", window);
        return -1;
    }

    sib1Nb_schedule(cellId, sib1Sched, NULL, NULL, NULL);

    sib2Nb_schedule(period, repetition, window);

    printf("SIB[1;31m1[0m-NB, SIB[1;33m2[0m-NB scheduling:\n\n");
    printf("     |  0  1  2  3  4  5  6  7  8  9\n");
    printf("-----+--------------------------------\n");
    for (i=0; i<1024; i++)
    {
        printf("%4d | ", i);
        for (j=0; j<10; j++)
        {
            n_abs_sf = ((i * 10) + j);

            if (0 == (n_abs_sf % window))
            {
                printf("[1;32m[[0m");
            }
            else
            {
                printf(" ");
            }

            if ( RESERVED_SUBFRAME(i, j) )
            {
                printf("x");
            }
            else if (g_sib1Subframe[i] & (0x1 << j))
            {
                printf("[1;31m1[0m");
            }
            else if (g_sib2Subframe[i] & (0x1 << j))
            {
                printf("[1;33m2[0m");
            }
            else
            {
                printf(" ");
            }

            if ((window - 1) == (n_abs_sf % window))
            {
                printf("[1;32m][0m");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");


    return 0;
}
