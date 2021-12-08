#include "types.h"

#define __LTE_CRC8_POLY__  (0x9B)  /* x^8 + x^7 + x^4 + x^3 + x + 1 */


static uint8 crc8_table[256] = {
    0x00, 0x9B, 0xAD, 0x36, 0xC1, 0x5A, 0x6C, 0xF7,
    0x19, 0x82, 0xB4, 0x2F, 0xD8, 0x43, 0x75, 0xEE,
    0x32, 0xA9, 0x9F, 0x04, 0xF3, 0x68, 0x5E, 0xC5,
    0x2B, 0xB0, 0x86, 0x1D, 0xEA, 0x71, 0x47, 0xDC,
    0x64, 0xFF, 0xC9, 0x52, 0xA5, 0x3E, 0x08, 0x93,
    0x7D, 0xE6, 0xD0, 0x4B, 0xBC, 0x27, 0x11, 0x8A,
    0x56, 0xCD, 0xFB, 0x60, 0x97, 0x0C, 0x3A, 0xA1,
    0x4F, 0xD4, 0xE2, 0x79, 0x8E, 0x15, 0x23, 0xB8,
    0xC8, 0x53, 0x65, 0xFE, 0x09, 0x92, 0xA4, 0x3F,
    0xD1, 0x4A, 0x7C, 0xE7, 0x10, 0x8B, 0xBD, 0x26,
    0xFA, 0x61, 0x57, 0xCC, 0x3B, 0xA0, 0x96, 0x0D,
    0xE3, 0x78, 0x4E, 0xD5, 0x22, 0xB9, 0x8F, 0x14,
    0xAC, 0x37, 0x01, 0x9A, 0x6D, 0xF6, 0xC0, 0x5B,
    0xB5, 0x2E, 0x18, 0x83, 0x74, 0xEF, 0xD9, 0x42,
    0x9E, 0x05, 0x33, 0xA8, 0x5F, 0xC4, 0xF2, 0x69,
    0x87, 0x1C, 0x2A, 0xB1, 0x46, 0xDD, 0xEB, 0x70,
    0x0B, 0x90, 0xA6, 0x3D, 0xCA, 0x51, 0x67, 0xFC,
    0x12, 0x89, 0xBF, 0x24, 0xD3, 0x48, 0x7E, 0xE5,
    0x39, 0xA2, 0x94, 0x0F, 0xF8, 0x63, 0x55, 0xCE,
    0x20, 0xBB, 0x8D, 0x16, 0xE1, 0x7A, 0x4C, 0xD7,
    0x6F, 0xF4, 0xC2, 0x59, 0xAE, 0x35, 0x03, 0x98,
    0x76, 0xED, 0xDB, 0x40, 0xB7, 0x2C, 0x1A, 0x81,
    0x5D, 0xC6, 0xF0, 0x6B, 0x9C, 0x07, 0x31, 0xAA,
    0x44, 0xDF, 0xE9, 0x72, 0x85, 0x1E, 0x28, 0xB3,
    0xC3, 0x58, 0x6E, 0xF5, 0x02, 0x99, 0xAF, 0x34,
    0xDA, 0x41, 0x77, 0xEC, 0x1B, 0x80, 0xB6, 0x2D,
    0xF1, 0x6A, 0x5C, 0xC7, 0x30, 0xAB, 0x9D, 0x06,
    0xE8, 0x73, 0x45, 0xDE, 0x29, 0xB2, 0x84, 0x1F,
    0xA7, 0x3C, 0x0A, 0x91, 0x66, 0xFD, 0xCB, 0x50,
    0xBE, 0x25, 0x13, 0x88, 0x7F, 0xE4, 0xD2, 0x49,
    0x95, 0x0E, 0x38, 0xA3, 0x54, 0xCF, 0xF9, 0x62,
    0x8C, 0x17, 0x21, 0xBA, 0x4D, 0xD6, 0xE0, 0x7B
};

void crc8_gentab(const uint8 polynomial)
{
    int accum;
    int i, j;

    printf("\n");
    printf("polynomial: x^8 ");
    for (i=0, j=7; i<8; i++, j--)
    {
        if (polynomial & (0x1 << j))
        {
            if (0 == j)
            {
                printf("+ 1");
            }
            else if (1 == j)
            {
                printf("+ x ");
            }
            else
            {
                printf("+ x^%d ", j);
            }
        }
    }
    printf("\n");
    printf("\n");

    printf("[CRC8 Table]\n");
    for (i=0; i<256; i++)
    {
        accum = i;

        for (j=0; j<8; j++)
        {
            if (accum & 0x80)
            {
                accum = (accum << 1) ^ polynomial;
            }
            else
            {
                accum = (accum << 1);
            }
        }

        crc8_table[i] = (uint8)accum;

        if ((i != 0) && ((i % 8) == 0))
        {
            printf("\n");
        }
        printf(" 0x%02X,", (int)crc8_table[i]);
    }
    printf("\n");
    printf("\n");
}

uint8 crc8_checksum(const uint8 *data, const int len)
{
    uint8 crc = 0;
    int i;

    for (i=0; i<len; i++)
    {
       crc = crc8_table[crc ^ data[i]];

       #if 0
       printf("[%d] %02X\n", i, crc);
       #endif
    }

    return crc;
}

void bit_reverse(uint8 *input, uint8 *output, int size)
{
    int i, j;

    for (i=0, j=size-1; i<size; i++, j--)
    {
        output[i] = 0;

        output[i] |= ((input[j] & 0x01) << 7);
        output[i] |= ((input[j] & 0x02) << 5);
        output[i] |= ((input[j] & 0x04) << 3);
        output[i] |= ((input[j] & 0x08) << 1);
        output[i] |= ((input[j] & 0x10) >> 1);
        output[i] |= ((input[j] & 0x20) >> 3);
        output[i] |= ((input[j] & 0x40) >> 5);
        output[i] |= ((input[j] & 0x80) >> 7);
    }
}

int main(int argc, char *argv[])
{
    uint8  polynomial = __LTE_CRC8_POLY__;
    uint8  rx_data[1024];
    int    rx_len;
    uint8  tx_data[] = {
        0x02, 0x14, 0xE5
        //CRC8: 0xE2
    };
    int    tx_len = sizeof( tx_data );
    uint8  crc = 0;
    uint8  crc_reverse = 0;


    //[1] generate CRC8 table
    if ((argc > 1) && (0 == strcmp("-g", argv[1])))
    {
        crc8_gentab( polynomial );
        return 0;
    }

    //[2] calculate CRC8 checksum
    printf("Data length %d\n\n", tx_len);
    crc = crc8_checksum(tx_data, tx_len);
    bit_reverse((uint8 *)&crc, (uint8 *)&crc_reverse, 1);
    printf("Calculate CRC8 : %02X (reversed %02X)\n", (int)crc, (int)crc_reverse);

    //[3] verify CRC8 checksum
    memcpy(rx_data, tx_data, tx_len);
    rx_data[tx_len] = crc;
    rx_len = tx_len + 1;
    crc = crc8_checksum(rx_data, rx_len);
    printf("Verify remainder : %02X\n", (int)crc);

    return 0;
}

