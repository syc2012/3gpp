#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_FLOOR(x, y)  ((x) / (y))


void help(void)
{
    printf("Usage: paging [OPTION]...\n");
    printf("\n");
    printf("  -t T           DRX cycle (32, 64, 128, 256).\n");
    printf("  -n N           Number of total paging frames (T/1, T/2, T/4, T/8, T/16).\n");
    printf("  -s Ns          Number of paging occasions (1, 2, 4).\n");
    printf("  -o PF_offset   Paging frame offset (0 ~ N-1).\n");
    printf("  -u UE_ID       UE_ID (0 ~ 1023).\n");
    printf("  -v             Verbose mode.\n");
    printf("  -h             Show the help message.\n");
    printf("\n");
}

/* 38.304 Section 7.1 Discontinuous Reception for paging */
int main(int argc, char *argv[])
{
    /* PCCH-Config
     *  + defaultPagingCycle ...... T
     *  + nAndPagingFrameOffset ... N, PF_offset
     *     + oneT
     *     + halfT
     *     + quarterT
     *     + oneEighthT
     *     + oneSixteenthT
     *  + ns ...................... Ns
     */
    int T = 64;
    int N = 1;
    int Ns = 1;
    int PF_offset = 0;
    /* UE_ID = ( 5G-S-TMSI mod 1024 ) */
    int UE_ID = 0;
    int verbose = 0;
    int SFN;
    int i_s;
    int ch;
    int i;
    int j;


    opterr = 0;
    while ((ch=getopt(argc, argv, "t:n:s:o:u:vh")) != -1)
    {
        switch ( ch )
        {
            case 't':
                T = atoi( optarg );
                break;
            case 'n':
                N = atoi( optarg );
                break;
            case 's':
                Ns = atoi( optarg );
                break;
            case 'o':
                PF_offset = atoi( optarg );
                break;
            case 'u':
                UE_ID = atoi( optarg );
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ((T != 32) && (T != 64) && (T != 128) && (T != 256))
    {
        printf("ERR: wrong T (%d)\n\n", T);
        return -1;
    }
    if ((N != 1) && (N != 2) && (N != 4) && (N != 8) && (N != 16))
    {
        printf("ERR: wrong N (%d)\n\n", N);
        return -1;
    }
    if ((Ns != 1) && (Ns != 2) && (Ns != 4))
    {
        printf("ERR: wrong Ns (%d)\n\n", Ns);
        return -1;
    }
    if ((PF_offset < 0) || (PF_offset > (N - 1)))
    {
        printf("ERR: wrong PF_offset (%d)\n\n", PF_offset);
        return -1;
    }
    if ((UE_ID < 0) || (UE_ID > 1023))
    {
        printf("ERR: wrong UE_ID (%d)\n\n", T);
        return -1;
    }


    printf("        T  %d (DRX cycle)\n", T);
    printf("        N  %d (%d paging frames)\n", N, (T / N));
    printf("       Ns  %d (paging occasions)\n", Ns);
    printf("PF_offset  %d\n", PF_offset);
    printf("    UE_ID  %d\n\n", UE_ID);


    N = (T / N);
    SFN = 0;

    i_s = (DIV_FLOOR(UE_ID, N) % Ns);
    printf("i_s  [1;35m%d[0m (Index)\n", i_s);
    printf("SFN\n");
    for (i=0; i<(1024/T); i++)
    {
        for (j=0; j<T; j++)
        {
            if (((SFN + PF_offset) % T) == ((T / N) * (UE_ID % N)))
            {
                printf(" [1;33m%d[0m", SFN);
            }
            else
            {
                if ( verbose ) printf(" %d", SFN);
            }
            SFN++;
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

