[ Test Algorithm ]

K
 ==> CK, IK, AK
      ==> K_AUSF
           ==> K_SEAF
                ==> K_AMF
                     ==> K_NAS_int, K_NAS_enc
                     ==> K_gNB, NH
                          ==> K_RRC_int, K_RRC_enc, K_UP_int, K_UP_enc
                          ==> K_NG_RAN_star

K        : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
RAND     : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
SQN      : 00 00 00 00 00 FF
AMF      : 90 01

CK       : 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00
IK       : 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00 01
AK       : 03 04 05 06 07 08

XRES size: 16
XRES     : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
XRES*    : 15 72 48 EF 1B 85 2F 19 C3 E6 A5 E5 BE DB 67 5C

XRES size: 8
XRES     : 00 01 02 03 04 05 06 07
XRES*    : 9E 81 8A 3E AD A0 6E 2D AC 63 88 C1 89 04 E6 D5

PLMN ID  : 001.01
K_AUSF   : 20 DE 4E B2 B7 5C 8D AC 30 1A D5 27 73 E2 E3 7B
         : D7 B4 B0 C2 FD B1 C3 75 1B 98 DD 0D F8 46 AE D5

K_SEAF   : D2 BB C6 3B 0C 14 12 47 34 1C 0F CA 3D C3 BF FD
         : C6 BF C1 81 34 89 34 2D E8 48 7E 7E 4D C3 72 8B

IMSI     : 001.01.0123456789
K_AMF    : 7E 9F 97 70 7C EF 48 31 8D F1 82 A7 F9 E7 BA 83
         : 8A 6F B3 6A B8 3A 7D 1B AF 26 10 B9 7A 5B 9D CF

Algorithm type    : 1
Algorithm identity: 2 (128-5G-EA2)
K_NAS_enc: 1C 3A 9D E3 B2 C5 56 A0 00 E1 8D F3 33 EC 8C BB

Algorithm type    : 2
Algorithm identity: 2 (128-5G-IA2)
K_NAS_int: 42 5D F2 AB 21 39 ED 36 BA F2 C3 0A 64 1F 94 F9

UL NAS count: 0
K_gNB    : 61 7C 2F 3D B7 14 A5 A1 05 97 59 1A 25 FE B2 1A
         : 15 9E A7 79 61 09 29 2B 0B 2C A5 79 D9 00 2B 0C

Algorithm type    : 3
Algorithm identity: 2 (128-5G-EA2)
K_RRC_enc: D5 29 44 83 F8 89 7C 0F B8 72 6C CA 75 DD 5E 37

Algorithm type    : 4
Algorithm identity: 2 (128-5G-IA2)
K_RRC_int: 6D 1A 1A 3F F5 4D 89 E7 C6 2C 32 EB FE 22 71 C5

Algorithm type    : 5
Algorithm identity: 2 (128-5G-EA2)
K_UP_enc : FB 41 3E 52 B2 15 4E 60 31 1F B3 A5 AE 9B C6 F5

Algorithm type    : 6
Algorithm identity: 2 (128-5G-IA2)
K_UP_int : 4B 43 D9 17 3D 1C 38 5A 29 38 E9 FC 74 B6 E8 B9

PCI      : 0
ARFCN-DL : 630400 (3456000 KHz)
K_NG_RAN_star
         : 62 68 14 23 78 18 36 BA 89 9A 54 48 CC E5 2B ED
         : 15 F7 C7 C9 8B 34 C9 A9 52 8E 27 7E 3B 19 D4 EA
