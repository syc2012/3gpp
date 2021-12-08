#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

#define WS_DLL_PUBLIC
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
typedef          int   gint;
typedef unsigned char  guint8;
typedef unsigned short guint16;
typedef unsigned int   guint32;
typedef unsigned char  gboolean;
typedef struct _packet_info
{
    int dummy;
} packet_info;
typedef struct _proto_item
{
    int dummy;
} proto_item;
#include "packet-pdcp-nr.h"


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

struct sockaddr_in g_sendAddr;
int   g_sockfd;

guint8 g_frameBuffer[1500];

guint8 g_pdcpPdu1[] = {
#if 1
 0x82, 0x46, 0xca, 
 0x45, 0x00, 0x00, 0x3c, 0xa8, 0xb7, 0x00, 0x00, 0x7f, 0x01, 0xfe, 0x32, 0xc0,
 0xa8, 0x08, 0x84, 0xc0, 0xa8, 0x0b, 0x02, 0x08, 0x00, 0x34, 0x08, 0x00, 0x01, 0x19, 0x53, 0x61,
 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69
#else
 0x80, 0x00,
 0x45, 0x00, 0x00, 0x54, 0x00, 0x00,
 0x40, 0x00, 0x40, 0x01, 0xb5, 0x4a, 0xc0, 0xa8,
 0x02, 0x02, 0xc0, 0xa8, 0x02, 0x0c, 0x08, 0x00,
 0x2e, 0x13, 0x7a, 0x3f, 0x00, 0x01, 0x1b, 0x78,
 0xda, 0x55, 0x00, 0x00, 0x00, 0x00, 0x98, 0x0b,
 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11,
 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,
 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
#endif
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void wireshark_nrPDCP(
    guint8    plane,
    guint8    snBits,
    guint8    direction,
    guint8    bearerType,
    guint8    bearerId,
    guint16   ueId,
    gboolean  maciFlag,
    gboolean  sdapFlag,
    guint16   pduLen,
    guint8   *pPdu
)
{
    guint8 *pBuf = g_frameBuffer;
    int bufLen;
    ssize_t sent;


    strcpy((char *)pBuf, PDCP_NR_START_STRING);
    pBuf += strlen(PDCP_NR_START_STRING);

    pBuf[0] = plane;
    pBuf++;

    pBuf[0] = PDCP_NR_SEQNUM_LENGTH_TAG;
    pBuf[1] = snBits;
    pBuf += 2;

    pBuf[0] = PDCP_NR_DIRECTION_TAG;
    pBuf[1] = direction;
    pBuf += 2;

    pBuf[0] = PDCP_NR_BEARER_TYPE_TAG;
    pBuf[1] = bearerType;
    pBuf += 2;

    pBuf[0] = PDCP_NR_BEARER_ID_TAG;
    pBuf[1] = bearerId;
    pBuf += 2;

    pBuf[0] = PDCP_NR_UEID_TAG;
    pBuf[1] = ((ueId >> 8) & 0xFF);
    pBuf[2] = (ueId & 0xFF);
    pBuf += 3;

    if ( maciFlag )
    {
        pBuf[0] = PDCP_NR_MACI_PRES_TAG;
        pBuf++;
    }

    if ( sdapFlag )
    {
        pBuf[0] = PDCP_NR_SDAP_HEADER_TAG;
        pBuf[1] = (PDCP_NR_DIRECTION_UPLINK == direction)
                   ? PDCP_NR_UL_SDAP_HEADER_PRESENT
                   : PDCP_NR_DL_SDAP_HEADER_PRESENT;
        pBuf += 2;
    }

    pBuf[0] = PDCP_NR_PAYLOAD_TAG;
    pBuf++;

    memcpy(pBuf, pPdu, pduLen);
    pBuf += pduLen;

    /* Send out the data over the UDP socket */
    bufLen = (pBuf - g_frameBuffer);
    sent = sendto(
               g_sockfd,
               g_frameBuffer,
               bufLen,
               0,
               (const struct sockaddr *)&g_sendAddr,
               sizeof(g_sendAddr)
           );
    if (sent != bufLen)
    {
        printf(
            "fail to send UDP packet (%d <---> %d)\n",
            bufLen,
            (int)sent
        );
    }
}

int main(int argc, char *argv[])
{
    struct hostent *pHost;

    if (argc < 3)
    {
        printf("Usage: wireshark_pdcp IP_ADDRESS PORT_NUMBER\n\n");
        return 0;
    }

    g_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sockfd < 0)
    {
        perror( "socket" );
        return -1;
    }

    g_sendAddr.sin_family = AF_INET;
    pHost = gethostbyname( argv[1] );
    if (pHost == (struct hostent *)0)
    {
        printf("fail to get host %s\n", argv[1]);
        close( g_sockfd );
        return -1;
    }
    memcpy((void *)&g_sendAddr.sin_addr, (void *)pHost->h_addr, pHost->h_length);
    g_sendAddr.sin_port = htons( atoi( argv[2] ) );


    wireshark_nrPDCP(
        NR_USER_PLANE,
        18,    /* Sequence Number Length */
        PDCP_NR_DIRECTION_DOWNLINK,
        Bearer_BCCH_DL_SCH,
        5,     /* Bearer ID */
        4096,  /* UE ID */
        FALSE, /* MACI */
        FALSE, /* SDAP */
        sizeof( g_pdcpPdu1 ),
        g_pdcpPdu1
    );


    close( g_sockfd );

    return 0;
}

