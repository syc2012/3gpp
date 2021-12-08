#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define DIV_FLOOR(X, Y)  ((X) / (Y))


typedef struct _tBand
{
    /* Operating frequency in KHz */
    int  F_low[2];
    int  F_high[2];

    /* Subcarrier spacing in KHz */
    int  F_raster[2];
    int  SSB_SCS[2];

    /* GSCN range */
    int  SSB_first[2];
    int  SSB_last[2];
    int  SSB_step[2];
} tBand;

tBand g_band[280];

void band_init(void)
{
    memset(&(g_band[0]), 0, (sizeof( tBand ) * 100));
    /* n257 */
    g_band[257].F_low[0]  = 26500020;
    g_band[257].F_low[1]  = 26500080;
    g_band[257].F_high[0] = 29499960;
    g_band[257].F_high[1] = 29499960;
    g_band[257].F_raster[0] = 60;
    g_band[257].F_raster[1] = 120;
    g_band[257].SSB_SCS[0] = 120;
    g_band[257].SSB_SCS[1] = 240;
    g_band[257].SSB_first[0] = 22388;
    g_band[257].SSB_first[1] = 22390;
    g_band[257].SSB_last[0]  = 22558;
    g_band[257].SSB_last[1]  = 22556;
    g_band[257].SSB_step[0]  = 1;
    g_band[257].SSB_step[1]  = 2;
    /* n258 */
    g_band[258].F_low[0]  = 24250080;
    g_band[258].F_low[1]  = 24250080;
    g_band[258].F_high[0] = 27499980;
    g_band[258].F_high[1] = 27499920;
    g_band[258].F_raster[0] = 60;
    g_band[258].F_raster[1] = 120;
    g_band[258].SSB_SCS[0] = 120;
    g_band[258].SSB_SCS[1] = 240;
    g_band[258].SSB_first[0] = 22257;
    g_band[258].SSB_first[1] = 22258;
    g_band[258].SSB_last[0]  = 22443;
    g_band[258].SSB_last[1]  = 22442;
    g_band[258].SSB_step[0]  = 1;
    g_band[258].SSB_step[1]  = 2;
    /* n260 */
    g_band[260].F_low[0]  = 37000020;
    g_band[260].F_low[1]  = 37000080;
    g_band[260].F_high[0] = 39999960;
    g_band[260].F_high[1] = 39999960;
    g_band[260].F_raster[0] = 60;
    g_band[260].F_raster[1] = 120;
    g_band[260].SSB_SCS[0] = 120;
    g_band[260].SSB_SCS[1] = 240;
    g_band[260].SSB_first[0] = 22995;
    g_band[260].SSB_first[1] = 22996;
    g_band[260].SSB_last[0]  = 23166;
    g_band[260].SSB_last[1]  = 23164;
    g_band[260].SSB_step[0]  = 1;
    g_band[260].SSB_step[1]  = 2;
    /* n261 */
    g_band[261].F_low[0]  = 27500040;
    g_band[261].F_low[1]  = 27500040;
    g_band[261].F_high[0] = 28350000;
    g_band[261].F_high[1] = 28350000;
    g_band[261].F_raster[0] = 60;
    g_band[261].F_raster[1] = 120;
    g_band[261].SSB_SCS[0] = 120;
    g_band[261].SSB_SCS[1] = 240;
    g_band[261].SSB_first[0] = 22446;
    g_band[261].SSB_first[1] = 22446;
    g_band[261].SSB_last[0]  = 22492;
    g_band[261].SSB_last[1]  = 22490;
    g_band[261].SSB_step[0]  = 1;
    g_band[261].SSB_step[1]  = 2;
}

int bw2rb(int band, int bw, int scs)
{
    int rb = 0;

    if (60 == scs)
    {
        if (50 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 66;
            }
        }
        else if (100 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 132;
            }
        }
        else if (200 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 264;
            }
        }
    }
    else if (120 == scs)
    {
        if (50 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 32;
            }
        }
        else if (100 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 66;
            }
        }
        else if (200 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 132;
            }
        }
        else if (400 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 264;
            }
        }
    }

    return rb;
}

void show_frequency(
    unsigned int SS_REF,    /* KHz */
    unsigned int ss_low,    /* KHz */
    unsigned int ss_high,   /* KHz */
    int          ss_raster, /* KHz */
    int          F_low[2],  /* KHz */
    int          F_high[2], /* KHz */
    int          F_raster[2],
    unsigned int centFreq,  /* MHz */
    int          BW,
    int          N_RB,
    int          SCS,
    int          j
)
{
    char raster[20];
    unsigned int F_REF;   /* KHz */
    unsigned int f_low;   /* KHz */
    unsigned int f_high;  /* KHz */
    unsigned int rb_low;  /* KHz */
    unsigned int rb_high; /* KHz */
    int N_SSB_CRB;
    int k_SSB;
    int count;
    int found;


    printf("================================================================\n");
    printf("        Frequency (MHz)        | N_SSB_CRB | k_SSB | Raster\n");
    printf("================================================================\n");
    printf(
        "[1;36mSSB[0m %.2f [1;33m%.2f[0m %.2f |           |       | %d KHz\n",
        ((double)ss_low / 1000),
        ((double)SS_REF / 1000),
        ((double)ss_high / 1000),
        ss_raster
       );
    printf("-------------------------------+-----------+-------+------------\n");
    found = 0;
    {
        count = 0;
        sprintf(raster, "%d KHz", F_raster[j]);

        for (F_REF = F_low[j]; F_REF <= F_high[j]; F_REF += F_raster[j])
        {
            f_low  = (F_REF - ((BW * 1000) / 2));
            f_high = (F_REF + ((BW * 1000) / 2));
            if (1 == (N_RB % 2))
            {
                rb_low  = (F_REF - (((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS));
                rb_high = (F_REF + (((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS));
            }
            else
            {
                rb_low  = (F_REF - ((DIV_FLOOR(N_RB, 2) * 12) * SCS));
                rb_high = (F_REF + ((DIV_FLOOR(N_RB, 2) * 12) * SCS));
            }
            if ((rb_low <= ss_low) && (ss_high <= rb_high))
            {
                #if 0
                printf(
                    "    %u %u %u (%d)\n",
                    rb_low,
                    F_REF,
                    rb_high,
                    ((ss_low - rb_low) % SCS)
                );
                continue;
                #endif
                if (0 == ((ss_low - rb_low) % SCS))
                {
                    int offset = ((ss_low - rb_low) / 60);
                    int sub = ((SCS / 60) * 12);
                    N_SSB_CRB = ((offset / sub) * (SCS / 60));
                    k_SSB = (offset % sub);
                    if (centFreq > 0)
                    {
                        if ((centFreq * 1000) == F_REF)
                        {
                            printf(
                                "[1;36mCRB[0m %.2f [1;33m%.2f[0m %.2f |    %4d   |   %2d  | %s\n",
                                ((double)rb_low / 1000),
                                ((double)F_REF / 1000),
                                ((double)rb_high / 1000),
                                N_SSB_CRB,
                                k_SSB,
                                raster
                            );
                            count++;
                            found = 1;
                            break;
                        }
                    }
                    else
                    {
                        printf(
                            "[1;36m%s[0m %.2f %.2f %.2f |    %4d   |   %2d  | %s\n",
                            ((count == 0) ? "CRB" : "   "),
                            ((double)rb_low / 1000),
                            ((double)F_REF / 1000),
                            ((double)rb_high / 1000),
                            N_SSB_CRB,
                            k_SSB,
                            ((count == 0) ? raster : "")
                        );
                        count++;
                    }
                }
            }
        }
        if (count > 0)
        {
            printf("-------------------------------+-----------+-------+------------\n");
        }
    }
    if ((centFreq > 0) && (0 == found))
    {
        printf("\n%u MHz is not an available center frequency\n", centFreq);
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: band_FR2 [OPTION]...\n");
    printf("\n");
    printf("  -b   NR operating band (257, 258, 260, 261).\n");
    printf("  -w   Bandwidth in MHz (50, 100, 200, 400).\n");
    printf("  -f   Center frequency in MHz (24250 ~ 52600).\n");
    printf("  -s   Subcarrier spacing (60, 120 KHz).\n");
    printf("  -g   GSCN (22256 ~ 26639).\n");
    printf("  -r   SS Block raster (120, 240 KHz).\n");
    printf("  -d   Delta-F raster (60, 120 KHz).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3 Synchronization raster */
int main(int argc, char *argv[])
{
    unsigned int SS_REF;  /* KHz */
    unsigned int ss_low;  /* KHz */
    unsigned int ss_high; /* KHz */
    unsigned int centFreq = 0; /* MHz */
    int band = 257;
    int GSCN = 22388;
    int BW = 50;
    int SCS = 60;  /* subCarrierSpacingCommon */
    int ss_raster = 120;
    int f_raster = 60;
    int g = 0;
    int N_RB;
    int N;
    int n;
    int i;
    int j;
    int ch;


    band_init();

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:w:f:s:g:r:d:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                band = atoi( optarg );
                break;
            case 'w':
                BW = atoi( optarg );
                break;
            case 'f':
                centFreq = (unsigned int)atoi( optarg );
                break;
            case 's':
                SCS = atoi( optarg );
                break;
            case 'g':
                g = atoi( optarg );
                break;
            case 'r':
                ss_raster = atoi( optarg );
                break;
            case 'd':
                f_raster = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    n = band;
    if ((n != 257) && (n != 258) && (n != 260) && (n != 261))
    {
        printf("ERR: wrong operating band %d\n", n);
        return 0;
    }

    for (i=0; i<2; i++)
    {
        //printf("g_band[%d].SSB_SCS[%d] = %d\n", n, i, g_band[n].SSB_SCS[i]);
        if ((g_band[n].SSB_SCS[i] > 0) &&
            (g_band[n].SSB_SCS[i] == ss_raster))
        {
            break;
        }
    }
    if (i > 1)
    {
        printf("ERR: wrong SS Block raster %d KHz\n", ss_raster);
        return 0;
    }

    for (j=0; j<2; j++)
    {
        //printf("g_band[%d].F_raster[%d] = %d\n", n, j, g_band[n].F_raster[j]);
        if ((g_band[n].F_raster[j] > 0) &&
            (g_band[n].F_raster[j] == f_raster))
        {
            break;
        }
    }
    if (j > 1)
    {
        printf("ERR: wrong Delta-F raster %d KHz\n", f_raster);
        return 0;
    }


    GSCN = ((g > 0) ? g : g_band[n].SSB_first[i]);
    if ((GSCN < g_band[n].SSB_first[i]) || (GSCN > g_band[n].SSB_last[i]))
    {
        printf("ERR: wrong GSCN %d in band %d\n", GSCN, band);
        return 0;
    }

    N_RB = bw2rb(band, BW, SCS);
    if (0 == N_RB)
    {
        printf("ERR: wrong bandwidth %d\n", BW);
        return 0;
    }

    printf("[ Band [1;31m%d[0m ]\n", band);
    printf("  Range  : %.2f ~ %.2f MHz (%.2f MHz)\n",
        ((double)g_band[n].F_low[0] / 1000),
        ((double)g_band[n].F_high[0] / 1000),
        ((double)(g_band[n].F_high[0] - g_band[n].F_low[0]) / 1000)
    );
    printf("  Delta-F: %d KHz\n", f_raster);
    printf("  BW     : %d MHz (%d RBs)\n", BW, N_RB);
    printf("  SCS    : %d KHz\n", SCS);
    printf("  GSCN   : %d\n", GSCN);
    printf("\n");

    if ((GSCN >= 22256) && (GSCN <= 26639))
    {
        N = (GSCN - 22256);
        SS_REF  = (24250080 + (N * 17280));  /* KHz */
        ss_low  = (SS_REF - (ss_raster * 120));
        ss_high = (SS_REF + (ss_raster * 120));
        show_frequency(
            SS_REF,
            ss_low,
            ss_high,
            ss_raster,
            g_band[n].F_low,
            g_band[n].F_high,
            g_band[n].F_raster,
            centFreq,
            BW,
            N_RB,
            SCS,
            j
        );
    }
    else
    {
        printf("ERR: wrong GSCN %d\n", GSCN);
        return 0;
    }


    return 0;
}

