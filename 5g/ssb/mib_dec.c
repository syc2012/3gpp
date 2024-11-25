#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DECODE(mib, offset, bits) \
    ((mib >> offset) & ((0x1 << bits) - 1))


/* 38.331 BCCH-BCH-Message */
int main(int argc, char *argv[])
{
    char *pHex = "0x5d6104";
    unsigned int mib;


    if (argc > 1)
    {
        if (0 == strcmp("-h", argv[1]))
        {
            printf("Usage: mib_dec 3_BYTES_MIB_in_HEX\n\n");
            return 0;
        }
        pHex = argv[1];
    }

    mib = (unsigned int)strtol(pHex, NULL, 16);
    printf(
        "\nMIB: %02x %02x %02x\n",
        ((mib >> 16) & 0xff),
        ((mib >>  8) & 0xff),
        ((mib      ) & 0xff)
    );

    /*
    * MIB: 1 + 23 = 24 bits
    *
    * 0xxx xxxx xxxx xxxx xxxx xxx0
    * |||| |||| |||| |||| |||| ||||
    * |||| |||| |||| |||| |||| |||`- spare
    * |||| |||| |||| |||| |||| ||`-- intraFreqReselection
    * |||| |||| |||| |||| |||| |`--- cellBarred
    * |||| |||| |||| |||| |``` `---- pdcch-ConfigSIB1.searchSpaceZero (0 ~ 15)
    * |||| |||| |||| |``` `--------- pdcch-ConfigSIB1.controlResourceSetZero (0 ~ 15)
    * |||| |||| |||| `-------------- dmrs-TypeA-Position
    * |||| |||| ````---------------- ssb-SubcarrierOffset (0 ~ 15)
    * |||| |||`--------------------- subCarrierSpacingCommon 
    * |``` ```---------------------- systemFrameNumber (MSB 6-bit)
    * `----------------------------- messageClassExtension
    *
    */
    printf("\n");
    printf("messageClassExtension ..... %d\n", DECODE(mib, 23, 1));
    printf("systemFrameNumber ......... %d\n", DECODE(mib, 17, 6));
    printf("subCarrierSpacingCommon ... %d\n", DECODE(mib, 16, 1));
    printf("ssb-SubcarrierOffset ...... %d\n", DECODE(mib, 12, 4));
    printf("dmrs-TypeA-Position ....... %d\n", DECODE(mib, 11, 1));
    printf("controlResourceSetZero .... %d\n", DECODE(mib,  7, 4));
    printf("searchSpaceZero ........... %d\n", DECODE(mib,  3, 4));
    printf("cellBarred ................ %d\n", DECODE(mib,  2, 1));
    printf("intraFreqReselection ...... %d\n", DECODE(mib,  1, 1));
    printf("spare ..................... %d\n", DECODE(mib,  0, 1));
    printf("\n");

    return 0;
}
