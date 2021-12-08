#include "utility.h"


void decode_0_0_c_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int padding;
    int size;
    int val;

    printf("\n");
    printf("[ C-RNTI, CS-RNTI, MCS-C-RNTI ]\n");
    printf("\n");

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("UL DCI flag: %d (1 bit)\n", val);

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("Frequency hopping: %d (1 bit)\n", val);

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
    printf("TPC command: %d (2 bits)\n", val);

    padding = (bits - offset);
    if (padding > 0)
    {
        val = get_bits(dci, (offset + padding - 1), 1);
        offset += padding;
        if (val > 0)
        {
            printf("Padding (%d bits)\n", (padding - 1));
            printf("UL/SUL indicator: %d\n", val);
        }
        else
        {
            printf("Padding (%d bits)\n", padding);
        }
    }

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

void decode_0_0_tc_rnti(uint8 *dci, int bits, int bwp)
{
    int offset = 0;
    int padding;
    int size;
    int val;

    printf("\n");
    printf("[ TC-RNTI ]\n");
    printf("\n");

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("UL DCI flag: %d (1 bit)\n", val);

    size = g_fresBits[bwp];
    val = get_bits(dci, offset, size);
    offset += size;
    printf("FD resource assignment: %d ([1;33m%d[0m bits)\n", val, size);

    val = get_bits(dci, offset, 4);
    offset += 4;
    printf("TD resource assignment: %d (4 bits)\n", val);

    val = get_bits(dci, offset, 1);
    offset += 1;
    printf("Frequency hopping: %d (1 bit)\n", val);

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
    printf("TPC command: %d (2 bits)\n", val);

    padding = (bits - offset);
    if (padding > 0)
    {
        val = get_bits(dci, (offset + padding - 1), 1);
        offset += padding;
        if (val > 0)
        {
            printf("Padding (%d bits)\n", (padding - 1));
            printf("UL/SUL indicator: %d\n", val);
        }
        else
        {
            printf("Padding (%d bits)\n", padding);
        }
    }

    printf("\n");

    if (offset != bits)
    {
        printf("ERR: un-match DCI bits (%d <--> %d)\n\n", offset, bits);
    }
}

/* 36.212 Section 7.3.1.1.1 Format 0_0 */
int main(int argc, char *argv[])
{
    uint8 dci[128];
    int len;

    if (argc < 4)
    {
        printf("Usage: dci_decode_0_0 RNTI DCI BITS BWP\n");
        printf("\n");
        printf("dci_decode c 3d19d1ca0000 41 100\n");
        printf("\n");
        return 0;
    }

    /* MSB is the most left bit */
    len = hex_string(argv[2], dci, 128);
    #if 0
    mem_dump(dci, len);
    #endif

    if (0 == strcmp("c", argv[1]))
    {
        /* C-RNTI, CS-RNTI, MCS-C-RNTI */
        decode_0_0_c_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else if (0 == strcmp("tc", argv[1]))
    {
        /* TC-RNTI */
        decode_0_0_tc_rnti(dci, atoi( argv[3] ), atoi( argv[4] ));
    }
    else
    {
        printf("ERR: unknown RNTI type '%s'\n\n", argv[1]);
    }

    return 0;
}

