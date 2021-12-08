#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define ENCODE(asn1, mib, offset, bits) \
    (asn1 |= ((mib & ((0x1 << bits) - 1)) << offset))

/*
* MIB ::=                     SEQUENCE {
*     systemFrameNumber           BIT STRING (SIZE (6)),
*     subCarrierSpacingCommon     ENUMERATED {scs15or60, scs30or120},
*     ssb-SubcarrierOffset        INTEGER (0..15),
*     dmrs-TypeA-Position         ENUMERATED {pos2, pos3},
*     pdcch-ConfigSIB1            PDCCH-ConfigSIB1,
*     cellBarred                  ENUMERATED {barred, notBarred},
*     intraFreqReselection        ENUMERATED {allowed, notAllowed},
*     spare                       BIT STRING (SIZE (1))
* }
*
* PDCCH-ConfigSIB1 ::=        SEQUENCE {
*     controlResourceSetZero      ControlResourceSetZero,
*     searchSpaceZero             SearchSpaceZero
* }
*
* ControlResourceSetZero ::=  INTEGER (0..15)
* SearchSpaceZero ::=         INTEGER (0..15)
*/
typedef struct _tMIB
{
    int systemFrameNumber;
    int subCarrierSpacingCommon;
    int ssbSubcarrierOffset;
    int dmrsTypeAPosition;
    int controlResourceSetZero;
    int searchSpaceZero;
    int cellBarred;
    int intraFreqReselection;
    int spare;
} tMIB;


void help(void)
{
    printf("Usage: mib_enc [OPTION]...\n");
    printf("\n");
    printf("  -f FRAME      System frame number (0 ~ 1023).\n");
    printf("  -c SCS        Subcarrier spacing common (0, 1).\n");
    printf("  -o OFFSET     SSB subcarrier offset (0 ~ 23).\n");
    printf("  -d DMRS       DMRS type-A position (0, 1).\n");
    printf("  -r RESOURCE   CORESET zero (0 ~ 15).\n");
    printf("  -s SEARCH     Search space zero (0 ~ 15).\n");
    printf("  -b BARRED     Cell barred (0, 1).\n");
    printf("  -i INTRA      Intra-frequency reselection (0, 1).\n");
    printf("  -h            Show the help message.\n");
    printf("\n");
}

/* 38.331 BCCH-BCH-Message */
int main(int argc, char *argv[])
{
    tMIB mib;
    unsigned int asn1;
    int ch;


    mib.systemFrameNumber = (120 >> 4);
    mib.subCarrierSpacingCommon = 1;
    mib.ssbSubcarrierOffset = 6;
    mib.dmrsTypeAPosition = 0;
    mib.controlResourceSetZero = 2;
    mib.searchSpaceZero = 0;
    mib.cellBarred = 1;
    mib.intraFreqReselection = 0;
    mib.spare = 0;

    opterr = 0;
    while ((ch=getopt(argc, argv, "f:c:o:d:r:s:b:i:h")) != -1)
    {
        switch ( ch )
        {
            case 'f':
                mib.systemFrameNumber = (atoi( optarg ) >> 4);
                break;
            case 'c':
                mib.subCarrierSpacingCommon = atoi( optarg );
                break;
            case 'o':
                mib.ssbSubcarrierOffset = atoi( optarg );
                break;
            case 'd':
                mib.dmrsTypeAPosition = atoi( optarg );
                break;
            case 'r':
                mib.controlResourceSetZero = atoi( optarg );
                break;
            case 's':
                mib.searchSpaceZero = atoi( optarg );
                break;
            case 'b':
                mib.cellBarred = atoi( optarg );
                break;
            case 'i':
                mib.intraFreqReselection = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    printf("\n");
    printf("messageClassExtension ..... 0\n");
    printf("systemFrameNumber ......... %d\n", mib.systemFrameNumber);
    printf("subCarrierSpacingCommon ... %d\n", mib.subCarrierSpacingCommon);
    printf("ssb-SubcarrierOffset ...... %d\n", mib.ssbSubcarrierOffset);
    printf("dmrs-TypeA-Position ....... %d\n", mib.dmrsTypeAPosition);
    printf("controlResourceSetZero .... %d\n", mib.controlResourceSetZero);
    printf("searchSpaceZero ........... %d\n", mib.searchSpaceZero);
    printf("cellBarred ................ %d\n", mib.cellBarred);
    printf("intraFreqReselection ...... %d\n", mib.intraFreqReselection);
    printf("spare ..................... 0\n");
    printf("\n");

    asn1 = 0;
    ENCODE(asn1, mib.systemFrameNumber, 17, 6);
    ENCODE(asn1, mib.subCarrierSpacingCommon, 16, 1);
    ENCODE(asn1, mib.ssbSubcarrierOffset, 12, 4);
    ENCODE(asn1, mib.dmrsTypeAPosition, 11, 1);
    ENCODE(asn1, mib.controlResourceSetZero, 7, 4);
    ENCODE(asn1, mib.searchSpaceZero, 3, 4);
    ENCODE(asn1, mib.cellBarred, 2, 1);
    ENCODE(asn1, mib.intraFreqReselection, 1, 1);
    ENCODE(asn1, mib.spare, 0, 1);

    printf(
        "MIB: %02x %02x %02x\n\n",
        ((asn1 >> 16) & 0xff),
        ((asn1 >>  8) & 0xff),
        ((asn1      ) & 0xff)
    );

    return 0;
}
