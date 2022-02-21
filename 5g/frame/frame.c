#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int g_scs[] = { 15, 30, 60, 120, 240 };
int g_nSlot[] = { 10, 20, 40, 80, 160 };

int main(int argc, char *argv[])
{
    int subframe;
    int slot;
    int symbol;
    int u;


    if (argc != 2)
    {
        printf("Usage: frame NUMEROLOGY(0-4)\n\n");
        return 0;
    }

    u = atoi( argv[1] );
    if ((u < 0) || (u > 4))
    {
        printf("ERR: wrong numerology %u\n\n", u);
        return 0;
    }


    printf("\n");
    printf(" Frame Subframe Slot Symbol\n");
    for (slot=0; slot<g_nSlot[u]; slot++)
    {
        subframe = (slot >> u);
        for (symbol=0; symbol<14; symbol++)
        {
            if (0 == symbol)
            {
                printf("0-1023 ");
            }
            else
            {
                printf("       ");
            }

            if (0 == symbol)
            {
                printf("%8d ", subframe);
            }
            else
            {
                printf("         ");
            }

            if (0 == symbol)
            {
                printf("%4d ", slot);
            }
            else
            {
                printf("     ");
            }

            printf("%6d\n", ((slot * 14) + symbol));
        }
    }
    printf("\n");
    printf("Subcarrier spacing: [1;33m%d[0m KHz\n\n", g_scs[u]);

    return 0;
}

