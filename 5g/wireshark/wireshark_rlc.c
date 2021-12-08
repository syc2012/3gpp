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

typedef unsigned char  guint8;
typedef unsigned short guint16;
typedef unsigned int   guint32;
typedef unsigned char  gboolean;
typedef struct _packet_info
{
    int dummy;
} packet_info;
#include "packet-rlc-nr.h"


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

struct sockaddr_in g_sendAddr;
int   g_sockfd;

guint8 g_frameBuffer[1500];

guint8 g_rlcPdu1[] = {
 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF
};

guint8 g_rlcPdu2[] = {
 0x00, 0x00, 0x00
};

guint8 g_rlcPdu3[] = {
 0x88, 0x00, 0x80, 0x00, 0x00, 0x01, 0x08, 0x00,
 0x06, 0x04, 0x00, 0x01, 0x00, 0xFF, 0x1D, 0xA1,
 0x3D, 0x28, 0xC0, 0xA8, 0x00, 0x01, 0x00, 0x00
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void wireshark_nrRLC(
    guint8   rlcMode,
    guint8   snBits,
    guint8   direction,
    guint8   bearerType,
    guint8   bearerId,
    guint16  ueId,
    guint16  pduLen,
    guint8  *pPdu
)
{
    guint8 *pBuf = g_frameBuffer;
    int bufLen;
    ssize_t sent;


    strcpy((char *)pBuf, RLC_NR_START_STRING);
    pBuf += strlen(RLC_NR_START_STRING);

    pBuf[0] = rlcMode;
    pBuf++;

    pBuf[0] = snBits;
    pBuf++;

    pBuf[0] = RLC_NR_DIRECTION_TAG;
    pBuf[1] = direction;
    pBuf += 2;

    pBuf[0] = RLC_NR_BEARER_TYPE_TAG;
    pBuf[1] = bearerType;
    pBuf += 2;

    pBuf[0] = RLC_NR_BEARER_ID_TAG;
    pBuf[1] = bearerId;
    pBuf += 2;

    pBuf[0] = RLC_NR_UEID_TAG;
    pBuf[1] = ((ueId >> 8) & 0xFF);
    pBuf[2] = (ueId & 0xFF);
    pBuf += 3;

    pBuf[0] = RLC_NR_PAYLOAD_TAG;
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
        printf("Usage: wireshark_rlc IP_ADDRESS PORT_NUMBER\n\n");
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


    wireshark_nrRLC(
        RLC_TM_MODE,
        12,  /* Sequence Number Length */
        DIRECTION_DOWNLINK,
        BEARER_TYPE_CCCH,
        1,   /* Bearer ID */
        168, /* UE ID */
        sizeof( g_rlcPdu1 ),
        g_rlcPdu1
    );

    wireshark_nrRLC(
        RLC_AM_MODE,
        12,  /* Sequence Number Length */
        DIRECTION_UPLINK,
        BEARER_TYPE_DRB,
        2,   /* Bearer ID */
        168, /* UE ID */
        sizeof( g_rlcPdu2 ),
        g_rlcPdu2
    );

    wireshark_nrRLC(
        RLC_AM_MODE,
        12,  /* Sequence Number Length */
        DIRECTION_UPLINK,
        BEARER_TYPE_DRB,
        3,   /* Bearer ID */
        168, /* UE ID */
        sizeof( g_rlcPdu3 ),
        g_rlcPdu3
    );


    close( g_sockfd );

    return 0;
}

