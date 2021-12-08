#include "utility.h"


void decode_1_0_c_rnti(uint8 *dci, int bits, int bwp, int raFlag)
{
    int offset = 0;
    int size;
    int val;

    printf("\n");
    printf("[ C-RNTI, CS-RNTI, MCS-C-RNTI ]\n");
    printf("\n");

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("DL DCI flag: %d (1 bit)\n", val);

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    if ( raFlag )
    {
        val = get_bits(dci, offset, 6);
        offset += 6;
        printf("RA preamble index: %d (6 bits)\n", val);

        val = get_bits(dci, offset, 1);
        offset += 1;
        printf("UL/SUL indicator: %d (1 bit)\n", val);

        val = get_bits(dci, offset, 6);
        offset += 6;
        printf("SS/PBCH index: %d (6 bits)\n", val);

        val = get_bits(dci, offset, 4);
        offset += 4;
        printf("PRACH mask index: %d (4 bits)\n", val);

        val = get_bits(dci, offset, 10);
        offset += 10;
        printf("Reserved: %d (10 bits)\n", val);
    }
    else
    {
        val = get_bits(dci, offset, 4);
        offset += 4;
        printf("TD resource assignment: %d (4 bits)\n", val);

        val = get_bits(dci, offset, 1);
        offset += 1;
        printf("VRB-PRB mapping: %d (1 bit)\n", val);

        val = get_bits(dci, offset, 5);
        offset += 5;
        printf("MCS: %d (5 bits)\n", val);

        val = get_bits(dci, offset, 1);
        offset += 1;
        printf("NDI: %d (1 bit)\n", val);

        val = get_bits(dci, offset, 2);
        offset += 2;
        printf("RV: %d (2 bits)\n", val);

        val = get_bits(dci, offset, 4);
        offset += 4;
        printf("HARQ ID: %d (4 bits)\n", val);

        val = get_bits(dci, offset, 2);
        offset += 2;
        printf("DAI: %d (2 bits)\n", val);

        val = get_bits(dci, offset, 2);
        offset += 2;
        printf("TPC command: %d (2 bits)\n", val);

        val = get_bits(dci, offset, 3);
        offset += 3;
        printf("PUCCH resource: %d (3 bits)\n", val);

        val = get_bits(dci, offset, 3);
        offset += 3;
        printf("PDSCH to HARQ feedback: %d (3 bits)\n", val);
    }

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

void decode_1_0_p_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int size;
    int val;

    printf("\n");
    printf("[ P-RNTI ]\n");
    printf("\n");

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("Short message indicator: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 8);
    offset += 8;
    printf("Short message: %02Xh (8 bits)\n", val);

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("VRB-PRB mapping: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 5);
    offset += 5;
    printf("MCS: %d (5 bits)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("TB scaling: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 6);
    offset += 6;
    printf("Reserved: %d (6 bits)\n", val);

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

void decode_1_0_si_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int size;
    int val;

    printf("\n");
    printf("[ SI-RNTI ]\n");
    printf("\n");

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("VRB-PRB mapping: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 5);
    offset += 5;
    printf("MCS: %d (5 bits)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("RV: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("SI indicator: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 15);
    offset += 15;
    printf("Reserved: %d (15 bits)\n", val);

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

void decode_1_0_ra_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int size;
    int val;

    printf("\n");
    printf("[ RA-RNTI ]\n");
    printf("\n");

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("VRB-PRB mapping: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 5);
    offset += 5;
    printf("MCS: %d (5 bits)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("TB scaling: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 16);
    offset += 16;
    printf("Reserved: %d (16 bits)\n", val);

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

void decode_1_0_tc_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int size;
    int val;

    printf("\n");
    printf("[ TC-RNTI ]\n");
    printf("\n");

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("DL DCI flag: %d (1 bit)\n", val);

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("VRB-PRB mapping: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 5);
    offset += 5;
    printf("MCS: %d (5 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("NDI: %d (1 bit)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("RV: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("HARQ ID: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("DAI: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 2);
    offset += 2;
    printf("TPC command: %d (2 bits)\n", val);

    val = get_bits(dci, offset, 3);
    offset += 3;
    printf("PUCCH resource: %d (3 bits)\n", val);

    val = get_bits(dci, offset, 3);
    offset += 3;
    printf("PDSCH to HARQ feedback: %d (3 bits)\n", val);

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

/* 36.212 Section 7.3.1.2.1 Format 1_0 */
int main(int argc, char *argv[])
{
    uint8 dci[128];
    int raFlag = 0;
    int len;

    if (argc < 4)
    {
        printf("Usage: dci_decode_1_0 RNTI DCI BITS BWP\n");
        printf("     : dci_decode_1_0 RNTI DCI BITS BWP RA\n");
        printf("\n");
        printf("dci_decode c bd1897880c80 41 100\n");
        printf("\n");
        return 0;
    }

    /* MSB is the most left bit */
    len = hex_string(argv[2], dci, 128);
    #if 0
    mem_dump(dci, len);
    #endif

    if (argc > 5)
    {
        raFlag = atoi( argv[5] );
    }

    if (0 == strcmp("c", argv[1]))
    {
        /* C-RNTI, CS-RNTI, MCS-C-RNTI */
        decode_1_0_c_rnti(dci, atoi( argv[3] ), atoi( argv[4] ), raFlag);
    }
    else if (0 == strcmp("p", argv[1]))
    {
        /* P-RNTI */
        decode_1_0_p_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else if (0 == strcmp("si", argv[1]))
    {
        /* SI-RNTI */
        decode_1_0_si_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else if (0 == strcmp("ra", argv[1]))
    {
        /* RA-RNTI */
        decode_1_0_ra_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else if (0 == strcmp("tc", argv[1]))
    {
        /* TC-RNTI */
        decode_1_0_tc_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else
    {
        printf("ERR: unknown RNTI type '%s'\n\n", argv[1]);
    }

    return 0;
}

