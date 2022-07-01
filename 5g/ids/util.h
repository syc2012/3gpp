#ifndef __UTIL_H__
#define __UTIL_H__


void str2plmn(char *pStr, unsigned char *pBuf);
void plmn2str(unsigned char *pBuf, char *pStr);

void int2hex(unsigned long long val, int bits, char *pStr);


#endif
