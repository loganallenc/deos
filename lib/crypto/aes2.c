/**********************************************************************
* Copyright (c) 2017 Andrew DeSantis                                  *
* Copyright (c) 2016 Pieter Wuille                                    *
* Distributed under the MIT software license, see the accompanying    *
* file COPYING or http://www.opensource.org/licenses/mit-license.php. *
***********************************************************************/

#include "crypto/aes2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* local declarations */
static void _loadbyte(aesstate* s, unsigned char byte, int r, int c);
static void _getonecolumn(aesstate* s, const aesstate* a, int c);
static void _subbytes(aesstate *s, int inv);
static void _keysetuptransform(aesstate* s, const aesstate* r);
static void _multx(aesstate* s);
static void _keysetupcolumnmix(aesstate* s, aesstate* r, const aesstate* a, int c1, int c2);
static void _loadbytes(aesstate *s, const unsigned char* data16);
static deosaes *__initdeosaes(int keysize, const uint8_t* key);

/* public functions */
deosaes *newdeosaes(int keysize, const uint8_t* key)
{
    deosaes *self = __initdeosaes(keysize, key);

    aesstate column;
    aesstate rcon = {{1,0,0,0,0,0,0,0}};

    int i;
    int pos = 0;

    if (self->keysize == 128)
    {   for (i=0; i<self->nrounds+1; i++)
        {   int b;
            for (b=0; b<8; b++) self->rounds[i].slice[b]=0;
        }
        for (i = 0; i < self->nkeywords; i++)
        {   int r;
            for (r = 0; r < 4; r++) _loadbyte(&self->rounds[i >> 2],
                                              *(key++),
                                              r,
                                              i & 3);
        }
        _getonecolumn(&column,
                      &self->rounds[(self->nkeywords - 1) >> 2],
                      (self->nkeywords - 1) & 3);
        for (i=self->nkeywords; i<4*(self->nrounds+1); i++)
        {   if (pos == 0)
            {   _subbytes(&column, 0);
                _keysetuptransform(&column, &rcon);
                _multx(&rcon);
            } else if (self->nkeywords>6 && pos==4)
                _subbytes(&column, 0);
            if (++pos==self->nkeywords) pos=0;
            _keysetupcolumnmix(&column,
                               &self->rounds[i >> 2],
                               &self->rounds[(i - self->nkeywords) >> 2],
                               i & 3, (i - self->nkeywords) & 3);
        }
    }
    return self;
}

static deosaes *__initdeosaes(int keysize, const uint8_t* key)
{
    deosaes *self = (deosaes *) malloc(sizeof(deosaes));
    if (self == NULL) return NULL;
    memset(self, 0, sizeof(deosaes));
    self->key = key;
    if (keysize == 128 || keysize == 192 || keysize == 256)
    {   self->keysize = keysize;
        if (self->keysize == 128)
        {   aes128ctx ctx;
            self->rounds = ctx.rk;
            self->nkeywords = 4;
            self->nrounds = 10;
        }
    }
    return self;
}

int deldeosaes(deosaes *self)
{
    if (NULL == self) return -1;
    free(self);
    return 0;
}

/* local functions */
static void _loadbyte(aesstate* s, unsigned char byte, int r, int c)
{
    int i;
    for (i = 0; i < 8; i++)
    {   s->slice[i] |= (byte & 1) << (r * 4 + c);
        byte >>= 1;
    }
}

static void _getonecolumn(aesstate* s, const aesstate* a, int c)
{
    int b;
    for (b = 0; b < 8; b++)
        s->slice[b] = (a->slice[b] >> c) & 0x1111;
}

static void _subbytes(aesstate *s, int inv)
{
    uint16_t U0 = s->slice[7], U1 = s->slice[6], U2 = s->slice[5], U3 = s->slice[4];
    uint16_t U4 = s->slice[3], U5 = s->slice[2], U6 = s->slice[1], U7 = s->slice[0];
    uint16_t T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16;
    uint16_t T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, D;
    uint16_t M1, M6, M11, M13, M15, M20, M21, M22, M23, M25, M37, M38, M39, M40;
    uint16_t M41, M42, M43, M44, M45, M46, M47, M48, M49, M50, M51, M52, M53, M54;
    uint16_t M55, M56, M57, M58, M59, M60, M61, M62, M63;
    if (inv)
    {   uint16_t R5, R13, R17, R18, R19;
        T23 =   U0 ^ U3;    T22 = ~(U1 ^ U3);  T2  = ~(U0 ^ U1);   T1  =   U3 ^ U4;
        T24 = ~(U4 ^ U7);   R5  =   U6 ^ U7;   T8  = ~(U1 ^ T23);  T19 =  T22 ^ R5;
        T9  = ~(U7 ^ T1);   T10 =   T2 ^ T24;  T13 =   T2 ^ R5;    T3  =   T1 ^ R5;
        T25 = ~(U2 ^ T1);   R13 =   U1 ^ U6;   T17 = ~(U2 ^ T19);  T20 =  T24 ^ R13;
        T4  =   U4 ^ T8;    R17 = ~(U2 ^ U5);  R18 = ~(U5 ^ U6);   R19 = ~(U2 ^ U4);
        D   =   U0 ^ R17;   T6  =  T22 ^ R17;  T16 =  R13 ^ R19;   T27 =   T1 ^ R18;
        T15 =  T10 ^ T27;   T14 =  T10 ^ R18;  T26 =   T3 ^ T16;
    } else
    {   T1  =   U0 ^ U3;    T2  =   U0 ^ U5;    T3 =   U0 ^ U6;     T4 =   U3 ^ U5;
        T5  =   U4 ^ U6;    T6  =   T1 ^ T5;    T7 =   U1 ^ U2;     T8 =   U7 ^ T6;
        T9  =   U7 ^ T7;    T10 =   T6 ^ T7;   T11 =   U1 ^ U5;    T12 =   U2 ^ U5;
        T13 =   T3 ^ T4;    T14 =   T6 ^ T11;  T15 =   T5 ^ T11;   T16 =   T5 ^ T12;
        T17 =   T9 ^ T16;   T18 =   U3 ^ U7;   T19 =   T7 ^ T18;   T20 =   T1 ^ T19;
        T21 =   U6 ^ U7;    T22 =   T7 ^ T21;  T23 =   T2 ^ T22;   T24 =   T2 ^ T10;
        T25 =  T20 ^ T17;   T26 =   T3 ^ T16;  T27 =   T1 ^ T12;   D   =   U7;
    }
    M1 = T13 & T6; M6 = T3 & T16; M11 = T1 & T15; M13 = (T4 & T27) ^ M11;
    M15 = (T2 & T10) ^ M11; M20 = T14 ^ M1 ^ (T23 & T8) ^ M13;
    M21 = (T19 & D) ^ M1 ^ T24 ^ M15; M22 = T26 ^ M6 ^ (T22 & T9) ^ M13;
    M23 = (T20 & T17) ^ M6 ^ M15 ^ T25; M25 = M22 & M20;
    M37 = M21 ^ ((M20 ^ M21) & (M23 ^ M25)); M38 = M20 ^ M25 ^ (M21 | (M20 & M23));
    M39 = M23 ^ ((M22 ^ M23) & (M21 ^ M25)); M40 = M22 ^ M25 ^ (M23 | (M21 & M22));
    M41 = M38 ^ M40; M42 = M37 ^ M39; M43 = M37 ^ M38; M44 = M39 ^ M40; M45 = M42 ^ M41;
    M46 = M44 & T6; M47 = M40 & T8; M48 = M39 & D; M49 = M43 & T16; M50 = M38 & T9;
    M51 = M37 & T17; M52 = M42 & T15; M53 = M45 & T27; M54 = M41 & T10; M55 = M44 & T13;
    M56 = M40 & T23; M57 = M39 & T19; M58 = M43 & T3; M59 = M38 & T22; M60 = M37 & T20;
    M61 = M42 & T1; M62 = M45 & T4; M63 = M41 & T2;
    if (inv)
    {
        uint16_t P0  = M52 ^ M61;  uint16_t P1  = M58 ^ M59;  uint16_t  P2 = M54 ^ M62;
        uint16_t P3  = M47 ^ M50;  uint16_t P4  = M48 ^ M56;  uint16_t  P5 = M46 ^ M51;
        uint16_t P6  = M49 ^ M60;  uint16_t P7  =  P0 ^ P1;   uint16_t  P8 = M50 ^ M53;
        uint16_t P9  = M55 ^ M63;  uint16_t P10 = M57 ^ P4;   uint16_t P11 =  P0 ^ P3;
        uint16_t P12 = M46 ^ M48;  uint16_t P13 = M49 ^ M51;  uint16_t P14 = M49 ^ M62;
        uint16_t P15 = M54 ^ M59;  uint16_t P16 = M57 ^ M61;  uint16_t P17 = M58 ^ P2;
        uint16_t P18 = M63 ^ P5;   uint16_t P19 =  P2 ^ P3;   uint16_t P20 =  P4 ^ P6;
        uint16_t P22 =  P2 ^ P7;   uint16_t P23 =  P7 ^ P8;   uint16_t P24 =  P5 ^ P7;
        uint16_t P25 =  P6 ^ P10;  uint16_t P26 =  P9 ^ P11;  uint16_t P27 = P10 ^ P18;
        uint16_t P28 = P11 ^ P25;  uint16_t P29 = P15 ^ P20;
        s->slice[7] = P13 ^ P22;  s->slice[6] = P26 ^ P29;  s->slice[5] = P17 ^ P28;
        s->slice[4] = P12 ^ P22;  s->slice[3] = P23 ^ P27;  s->slice[2] = P19 ^ P24;
        s->slice[1] = P14 ^ P23;  s->slice[0] =  P9 ^ P16;
    } else {
        uint16_t L0 = M61 ^ M62; uint16_t L1 = M50 ^ M56; uint16_t L2 = M46 ^ M48;
        uint16_t L3 = M47 ^ M55; uint16_t L4 = M54 ^ M58; uint16_t L5 = M49 ^ M61;
        uint16_t L6 = M62 ^ L5; uint16_t L7 = M46 ^ L3; uint16_t L8 = M51 ^ M59;
        uint16_t L9 = M52 ^ M53; uint16_t L10 = M53 ^ L4; uint16_t L11 = M60 ^ L2;
        uint16_t L12 = M48 ^ M51; uint16_t L13 = M50 ^ L0; uint16_t L14 = M52 ^ M61;
        uint16_t L15 = M55 ^ L1; uint16_t L16 = M56 ^ L0; uint16_t L17 = M57 ^ L1;
        uint16_t L18 = M58 ^ L8; uint16_t L19 = M63 ^ L4; uint16_t L20 = L0 ^ L1;
        uint16_t L21 = L1 ^ L7; uint16_t L22 = L3 ^ L12; uint16_t L23 = L18 ^ L2;
        uint16_t L24 = L15 ^ L9; uint16_t L25 = L6 ^ L10; uint16_t L26 = L7 ^ L9;
        uint16_t L27 = L8 ^ L10; uint16_t L28 = L11 ^ L14; uint16_t L29 = L11 ^ L17;
        s->slice[7] = L6 ^ L24; s->slice[6] = ~(L16 ^ L26); s->slice[5] = ~(L19 ^ L28);
        s->slice[4] = L6 ^ L21; s->slice[3] = L20 ^ L22; s->slice[2] = L25 ^ L29;
        s->slice[1] = ~(L13 ^ L27); s->slice[0] = ~(L6 ^ L23);
    }
}

static void _keysetuptransform(aesstate* s, const aesstate* r)
{
    int b;
    for (b = 0; b < 8; b++)
        s->slice[b] = ((s->slice[b] >> 4) | (s->slice[b] << 12)) ^ r->slice[b];
}

static void _multx(aesstate* s)
{
    uint16_t top = s->slice[7];
    s->slice[7] = s->slice[6];
    s->slice[6] = s->slice[5];
    s->slice[5] = s->slice[4];
    s->slice[4] = s->slice[3] ^ top;
    s->slice[3] = s->slice[2] ^ top;
    s->slice[2] = s->slice[1];
    s->slice[1] = s->slice[0] ^ top;
    s->slice[0] = top;
}

static void _keysetupcolumnmix(aesstate* s, aesstate* r, const aesstate* a, int c1, int c2)
{
    int b;
    for (b = 0; b < 8; b++)
        r->slice[b] |= ((s->slice[b] ^= ((a->slice[b] >> c2) & 0x1111)) & 0x1111) << c1;
}

static void _loadbytes(aesstate *s, const unsigned char* data16)
{
    int c;
    for (c = 0; c < 4; c++)
    {   int r;
        for (r = 0; r < 4; r++)
            _loadbyte(s, *(data16++), r, c);
    }
}

#ifdef __cplusplus
}
#endif
