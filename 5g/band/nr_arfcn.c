#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned int toArfcn(unsigned int F_REF)
{
    unsigned int N_REF = 0;
    unsigned int F_REF_Offs = 0;
    unsigned int F_Global = 0;
    unsigned int N_REF_Offs = 0;

    if (F_REF <= 3000000)
    {
        N_REF_Offs = 0;
        F_REF_Offs = 0;
        F_Global = 5;
    }
    else if (F_REF <= 24250000)
    {
        N_REF_Offs = 600000;
        F_REF_Offs = 3000000;
        F_Global = 15;
    }
    else if (F_REF <= 100000000)
    {
        N_REF_Offs = 2016667;
        F_REF_Offs = 24250080;
        F_Global = 60;
    }
    else
    {
        printf("ERR: wrong Frequency %u\n", F_REF);
        return 0;
    }

    if (((F_REF - F_REF_Offs) % F_Global) != 0)
    {
        printf("ERR: wrong Frequency %u\n", F_REF);
        return 0;
    }

    N_REF = (((F_REF - F_REF_Offs) / F_Global) + N_REF_Offs);

    return N_REF;
}

unsigned int toFreq(unsigned int N_REF)
{
    unsigned int F_REF = 0;
    unsigned int F_REF_Offs = 0;
    unsigned int F_Global = 0;
    unsigned int N_REF_Offs = 0;

    if (N_REF < 600000)
    {
        N_REF_Offs = 0;
        F_REF_Offs = 0;
        F_Global = 5;
    }
    else if (N_REF < 2016667)
    {
        N_REF_Offs = 600000;
        F_REF_Offs = 3000000;
        F_Global = 15;
    }
    else if (N_REF < 3279166)
    {
        N_REF_Offs = 2016667;
        F_REF_Offs = 24250080;
        F_Global = 60;
    }
    else
    {
        printf("ERR: wrong ARFCN %u\n", N_REF);
        return 0;
    }

    F_REF = (F_REF_Offs + (F_Global * (N_REF - N_REF_Offs)));

    return F_REF;
}

void help(void)
{
    printf("Usage: nr_arfcn -f FREQUENCY\n");
    printf("     : nr_arfcn -n ARFCN\n");
    printf("\n");
    printf("  -f   Frequency value (0 ~ 100000000 KHz).\n");
    printf("  -n   ARFCN value (0 ~ 3279165).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.2.1 NR-ARFCN and channel raster */
int main(int argc, char *argv[])
{
    unsigned int F_REF = 3350010;
    unsigned int N_REF = 623334;


    if (3 == argc)
    {
        if (0 == strcmp("-f", argv[1]))
        {
            F_REF = atoi( argv[2] );
            N_REF = toArfcn( F_REF );
            if (N_REF > 0)
            {
                printf("\nNR-ARFCN %u\n\n", N_REF);
            }
        }
        else if (0 == strcmp("-n", argv[1]))
        {
            N_REF = atoi( argv[2] );
            F_REF = toFreq( N_REF );
            if (F_REF > 0)
            {
                printf("\n%u.%03u MHz\n\n", (F_REF / 1000), (F_REF % 1000));
            }
        }
        else
        {
            help();
        }
    }
    else
    {
        help();
    }

    return 0;
}

