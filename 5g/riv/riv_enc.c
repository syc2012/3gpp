#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


int encodeRiv(int N_size_BWP, int RB_start, int L_RBs)
{
    int RIV = -1;

    if (N_size_BWP < 1) return -1;
    if (N_size_BWP > 275) return -1;
    if (L_RBs < 1) return -1;
    if (L_RBs > (N_size_BWP - RB_start)) return -1;

    if ((L_RBs - 1) <= DIV_FLOOR(N_size_BWP, 2))
    {
        RIV = ((N_size_BWP * (L_RBs - 1)) + RB_start);
    }
    else
    {
        RIV = ((N_size_BWP * (N_size_BWP - L_RBs + 1)) + (N_size_BWP - 1 - RB_start));
    }

    return RIV;
}

/* 38.214 Section 5.1.2.2.2 Downlink resource allocation type 1 */
int main(int argc, char *argv[])
{
    int N_size_BWP = 275;
    int RB_start = 0;
    int L_RBs = 50;
    int RIV;
    int bits;


    if (argc > 2)
    {
        RB_start = atoi( argv[1] );
        L_RBs = atoi( argv[2] );
        if (argc > 3)
        {
            N_size_BWP = atoi( argv[3] );
        }

        RIV = encodeRiv(N_size_BWP, RB_start, L_RBs);
        if (RIV < 0)
        {
            printf("ERR: wrong input values\n\n");
        }
        else
        {
            bits = ceil( log2((N_size_BWP * (N_size_BWP + 1)) / 2) );
            printf("\n");
            printf("BWP size = %d\n", N_size_BWP);
            printf("RIV bits = %d\n", bits);
            printf("RIV = %d (0x%X)\n", RIV, RIV);
            printf("\n");
        }
    }
    else
    {
        printf("Usage: riv_enc RB_start L_RBs\n");
        printf("     : riv_enc RB_start L_RBs N_size_BWP\n\n");
    }

    return 0;
}

