#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gsm7bit.h"


#if 0
/* 299 characters (262 bytes) */
uint8  pSmsString[] = "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message stringXY";
uint16 smsStringLen;
#endif

#if 0
/* 12 characters (11 bytes) */
uint8  pSmsString[] = "0123456789.X";
uint16 smsStringLen;
#endif

#if 0
/* 7 characters plus one escap character */
uint8  pSmsString[] = "Svc[123";
uint16 smsStringLen;
#endif

#if 0
/* 14 characters plus 6 escap characters */
uint8  pSmsString[] = "123{}[]\\$~1234";
uint16 smsStringLen;
#endif

#if 1
/* 90 characters (79 bytes) */
uint8  pSmsString[] = "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "abcdefghij";
uint16 smsStringLen;
#endif

#if 0
/* max length: 160 characters (140 bytes) */
uint8  pSmsString[] = "Abcdefghijklmnop"
                      "1111111111111111"
                      "2222222222222222"
                      "3333333333333333"
                      "4444444444444444"
                      "5555555555555555"
                      "6666666666666666"
                      "7777777777777777"
                      "8888888888888888"
                      "zyxwvutsrqponmlK";
uint16 smsStringLen;
#endif

int main(void)
{
    uint16  messageLen = 0;
    uint8   message[1250];

    smsStringLen = strlen( (char *)pSmsString );
    printf("String length = %u\n", smsStringLen);

    memset(message, 0x00, 1250);
    {
        /* GSM 7-bit default alphabet */
        messageLen = sms_encodeGsm7bit(
                         message,
                         1250,
                         pSmsString,
                         smsStringLen
                     );
    }

    printf("Encoded length = %u\n\n", messageLen);
    mem_dump(message, messageLen);

    return 0;
}

