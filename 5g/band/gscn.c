#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned int toGscn(unsigned int SS_REF, int *Mout, int *Nout)
{
    unsigned int GSCN = 0;
    int M = 0;
    int N = 0;

    if (SS_REF < 3000000)
    {
        if (((SS_REF % 1200) % 50) != 0)
        {
            printf("ERR: illegal frequency\n\n");
            return 0;
        }

        M = ((SS_REF % 1200) / 50);
        N = (SS_REF / 1200);

        if ((M != 1) && (M != 3) && (M != 5))
        {
            printf("ERR: M %d should be (1, 3, 5)\n\n", N);
            return 0;
        }
        if ((N < 1) || (N > 2499))
        {
            printf("ERR: N %d should be (1 ~ 2499)\n\n", N);
            return 0;
        }

        GSCN = ((3 * N) + ((M - 3) / 2));
    }
    else if (SS_REF < 24250000)
    {
        if (((SS_REF - 3000000) % 1440) != 0)
        {
            printf("ERR: illegal frequency\n\n");
            return 0;
        }

        N = ((SS_REF - 3000000) / 1440);

        if ((N < 0) || (N > 14756))
        {
            printf("ERR: N %d should be (0 ~ 14756)\n\n", N);
            return 0;
        }

        GSCN = (7499 + N);
    }
    else
    {
        if (((SS_REF - 24250080) % 17280) != 0)
        {
            printf("ERR: illegal frequency\n\n");
            return 0;
        }

        N = ((SS_REF - 24250080) / 17280);

        if ((N < 0) || (N > 4383))
        {
            printf("ERR: N %d should be (0 ~ 4383)\n\n", N);
            return 0;
        }

        GSCN = (22256 + N);
    }

    if ( Mout ) *Mout = M;
    if ( Nout ) *Nout = N;

    return GSCN;
}

unsigned int toFreq(unsigned int GSCN, int *Mout, int *Nout)
{
    unsigned int SS_REF = 0;
    int M = 0;
    int N = 0;

    if (GSCN < 7499)
    {
        if (1 == (GSCN % 3))
        {
            M = 5;  // +1
        }
        else if (2 == (GSCN % 3))
        {
            M = 1;  // -1
        }
        else
        {
            M = 3;
        }
        N = ((GSCN - ((M - 3) / 2)) / 3);

        if ((N < 1) || (N > 2499))
        {
            printf("ERR: N %d should be (1 ~ 2499)\n\n", N);
            return 0;
        }

        SS_REF = ((N * 1200) + (M * 50));
    }
    else if (GSCN < 22256)
    {
        N = (GSCN - 7499);

        if ((N < 0) || (N > 14756))
        {
            printf("ERR: N %d should be (0 ~ 14756)\n\n", N);
            return 0;
        }

        SS_REF = (3000000 + (N * 1440));
    }
    else
    {
        N = (GSCN - 22256);

        if ((N < 0) || (N > 4383))
        {
            printf("ERR: N %d should be (0 ~ 4383)\n\n", N);
            return 0;
        }

        SS_REF = (24250080 + (N * 17280));
    }

    if ( Mout ) *Mout = M;
    if ( Nout ) *Nout = N;

    return SS_REF;
}

void help(void)
{
    printf("Usage: gscn -f FREQUENCY\n");
    printf("     : gscn -n GSCN\n");
    printf("\n");
    printf("  -f   Frequency value (1250 ~ 99988320) KHz.\n");
    printf("  -n   GSCN value (2 ~ 26639).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3.1 Synchronization raster and numbering */
int main(int argc, char *argv[])
{
    unsigned int SS_REF = 3305280;
    unsigned int GSCN = 7711;
    int M;
    int N;


    if (3 == argc)
    {
        if (0 == strcmp("-f", argv[1]))
        {
            SS_REF = atoi( argv[2] );
            if ((SS_REF < 1250) || (SS_REF > 99988320))
            {
                printf("ERR: worng Frequency (%u)\n\n", SS_REF);
                return 0;
            }

            GSCN = toGscn(SS_REF, &M, &N);
            if (GSCN > 0)
            {
                printf("\nGSCN %u\n", GSCN);
                if (M > 0)
                {
                    printf("M %d\n", M);
                }
                printf("N %d\n\n", N);
            }
        }
        else if (0 == strcmp("-n", argv[1]))
        {
            GSCN = atoi( argv[2] );
            if ((GSCN < 2) || (GSCN > 26639))
            {
                printf("ERR: worng GSCN (%u)\n\n", GSCN);
                return 0;
            }

            SS_REF = toFreq(GSCN, &M, &N);
            if (SS_REF > 0)
            {
                printf("\n%u.%03u MHz\n", (SS_REF / 1000), (SS_REF % 1000));
                if (M > 0)
                {
                    printf("M %d\n", M);
                }
                printf("N %d\n\n", N);
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

