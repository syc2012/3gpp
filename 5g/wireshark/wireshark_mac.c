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
typedef unsigned char  guint8;
typedef unsigned short guint16;
typedef unsigned int   guint32;
typedef unsigned char  gboolean;
typedef struct _packet_info
{
    int dummy;
} packet_info;
#include "packet-mac-nr.h"


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

struct sockaddr_in g_sendAddr;
int   g_sockfd;

guint8 g_frameBuffer[1500];

guint8 g_macPdu1[] = {
 0x38, 0x2A
};

guint8 g_macPdu2[] = {
 0x40, 0x40, 0x04, 0x03, 0x00, 0xC9, 0x00, 0x00,
 0x00, 0x1C, 0x14, 0x01, 0x10, 0x20, 0x81, 0x88,
 0x22, 0xA0
};

guint8 g_macPdu3[] = {
 0x3e, 0x1f, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void wireshark_nrMAC(
    guint8    radioType,
    guint8    direction,
    guint8    rntiType,
    guint16   rnti,
    guint16   ueId,
    guint8    harqId,
    guint16   frame,
    guint16   slot,
    guint16   pduLen,
    guint8   *pPdu
)
{
    guint8 *pBuf = g_frameBuffer;
    int bufLen;
    ssize_t sent;


    strcpy((char *)pBuf, MAC_NR_START_STRING);
    pBuf += strlen(MAC_NR_START_STRING);

    pBuf[0] = radioType;
    pBuf++;

    pBuf[0] = direction;
    pBuf++;

    pBuf[0] = rntiType;
    pBuf++;

    pBuf[0] = MAC_NR_RNTI_TAG;
    pBuf[1] = ((rnti >> 8) & 0xFF);
    pBuf[2] = (rnti & 0xFF);
    pBuf += 3;

    pBuf[0] = MAC_NR_UEID_TAG;
    pBuf[1] = ((ueId >> 8) & 0xFF);
    pBuf[2] = (ueId & 0xFF);
    pBuf += 3;

    pBuf[0] = MAC_NR_HARQID;
    pBuf[1] = harqId;
    pBuf += 2;

    pBuf[0] = MAC_NR_FRAME_SLOT_TAG;
    pBuf[1] = ((frame >> 8) & 0xFF);
    pBuf[2] = (frame & 0xFF);
    pBuf[3] = ((slot >> 8) & 0xFF);
    pBuf[4] = (slot & 0xFF);
    pBuf += 5;

    pBuf[0] = MAC_NR_PAYLOAD_TAG;
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
        printf("Usage: wireshark_mac IP_ADDRESS PORT_NUMBER\n\n");
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


    wireshark_nrMAC(
        FDD_RADIO,
        DIRECTION_DOWNLINK,
        C_RNTI,
        4096, /* RNTI */
        4096, /* UE ID */
        1,    /* HARQ ID */
        20,   /* Frame */
        8,    /* Slot */
        sizeof( g_macPdu1 ),
        g_macPdu1
    );

    wireshark_nrMAC(
        FDD_RADIO,
        DIRECTION_DOWNLINK,
        SI_RNTI,
        65535, /* RNTI */
        0,     /* UE ID */
        0,     /* HARQ ID */
        4,     /* Frame */
        0,     /* Slot */
        sizeof( g_macPdu2 ),
        g_macPdu2
    );

    wireshark_nrMAC(
        FDD_RADIO,
        DIRECTION_UPLINK,
        C_RNTI,
        4098, /* RNTI */
        4098, /* UE ID */
        2,    /* HARQ ID */
        123,  /* Frame */
        9,    /* Slot */
        sizeof( g_macPdu3 ),
        g_macPdu3
    );


    close( g_sockfd );

    return 0;
}

