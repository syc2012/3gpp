#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

void help(void)
{
    printf("Usage: pointA [OPTION]...\n");
    printf("\n");
    printf("  -c   Carrier frequency (KHz).\n");
    printf("  -f   SSB center frequency (KHz).\n");
    printf("  -s   Subcarrier spacing common (KHz).\n");
    printf("  -r   RB number.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    /*
    * FR1:
    *  N_SSB_CRB 15KHz
    *  k_SSB     0 ~ 23 (15KHz)
    *
    * FR2:
    *  N_SSB_CRB 60KHz
    *  k_SSB     0 ~ 11 (subCarrierSpacingCommon)
    */
    unsigned long Fc = 3456000; /* KHz */
    unsigned long Fssb[3] = { 0, 3411840, 0 }; /* KHz */
    unsigned long Fpa; /* KHz */
    int FR1 = 1;
    int SCScommon = 30; /* KHz */
    int SCSpa = 15; /* KHz */
    int Nrb = 273;
    int subcarriers;
    int Nssbcrb;
    int Kssb;
    int opt;


    opterr = 0;
    while ((opt=getopt(argc, argv, "c:f:r:s:h")) != -1)
    {
        switch ( opt )
        {
            case 'c':
                Fc = strtoul(optarg, NULL, 10);
                break;
            case 'f':
                Fssb[1] = strtoul(optarg, NULL, 10);
                break;
            case 's':
                SCScommon = atoi( optarg );
                break;
            case 'r':
                Nrb = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if (Fc < 24250000)
    {
        FR1 = 1; /* FR1 */
        SCSpa = 15;
        if (15 == SCScommon)
        {
            switch ( Nrb )
            {
                case 25:  case 52:  case 79:  case 106: case 133:
                case 160: case 188: case 216: case 242: case 270:
                    break;
                default:
                    printf("ERR: incorrect FR1 RB number %d\n", Nrb);
                    return -1;
            }
        }
        else if (30 == SCScommon)
        {
            switch ( Nrb )
            {
                case 11:  case 24:  case 38:  case 51:  case 65:
                case 78:  case 92:  case 106: case 119: case 133:
                case 162: case 189: case 217: case 245: case 273:
                    break;
                default:
                    printf("ERR: incorrect FR1 RB number %d\n", Nrb);
                    return -1;
            }
        }
        else if (60 == SCScommon)
        {
            switch ( Nrb )
            {
                case 11:  case 18:  case 24:  case 31:  case 38:
                case 44:  case 51:  case 58:  case 65:  case 79:
                case 93:  case 107: case 121: case 135:
                    break;
                default:
                    printf("ERR: incorrect FR1 RB number %d\n", Nrb);
                    return -1;
            }
        }
        else
        {
            printf("ERR: incorrect FR1 SCS %d KHz\n", SCScommon);
            return -1;
        }

    }
    else
    {
        FR1 = 0; /* FR2 */
        SCSpa = 60;
        if (60 == SCScommon)
        {
            switch ( Nrb )
            {
                case 66:  case 132: case 264:
                    break;
                default:
                    printf("ERR: incorrect FR2 RB number %d\n", Nrb);
                    return -1;
            }
        }
        else if (120 == SCScommon)
        {
            switch ( Nrb )
            {
                case 32:  case 66:  case 132: case 264:
                    break;
                default:
                    printf("ERR: incorrect FR2 RB number %d\n", Nrb);
                    return -1;
            }
        }
        else
        {
            printf("ERR: incorrect FR2 SCS %d KHz\n", SCScommon);
            return -1;
        }
    }

    subcarriers = 120;
    Fssb[0] = Fssb[1] + (subcarriers * SCScommon);
    Fssb[2] = Fssb[1] - (subcarriers * SCScommon);

    if (Nrb & 0x1)
    {
        subcarriers = ((Nrb / 2) * 12) + 6;
        Fpa = Fc - (subcarriers * SCScommon);
    }
    else
    {
        subcarriers = ((Nrb / 2) * 12);
        Fpa = Fc - (subcarriers * SCScommon);
    }

    subcarriers = (Fssb[2] - Fpa) / SCScommon;
    Nssbcrb = (subcarriers / 12) * (SCScommon / SCSpa);
    Kssb = (subcarriers % 12) * (SCScommon / SCSpa);

    printf("\n");
    printf("  Subcarrier spacing: %d KHz (Common)\n", SCScommon);
    printf("   Carrier frequency: %f MHz (%s)\n", (double)(Fc)/1e3, (FR1 ? "FR1" : "FR2"));
    printf("SSB top    frequency: %f MHz\n", (double)(Fssb[0])/1e3);
    printf("SSB center frequency: %f MHz\n", (double)(Fssb[1])/1e3);
    printf("SSB bottom frequency: %f MHz\n", (double)(Fssb[2])/1e3);
    printf("             Point A: [1;33m%f[0m MHz\n", (double)(Fpa)/1e3);
    printf("                N_RB: %d\n", Nrb);
    printf("           N_SSB_CRB: [1;36m%d[0m\n", Nssbcrb);
    printf("               K_SSB: [1;36m%d[0m\n", Kssb);
    printf("\n");

    return 0;
}

