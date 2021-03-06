/**********************************************************************
* Copyright (c) 2017 Andrew DeSantis                                  *
* Copyright (c) 2016 Pieter Wuille                                    *
* Distributed under the MIT software license, see the accompanying    *
* file COPYING or http://www.opensource.org/licenses/mit-license.php. *
***********************************************************************/

#ifndef __deoscryptoaes
#define __deoscryptoaes

#ifdef __cplusplus
extern "C" {
#endif /*cplusplus*/

#ifndef __atdlib
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#endif /*atdlib*/

typedef struct
{   uint16_t slice[8];
} aesstate;

typedef struct
{   aesstate rk[11];
} aes128ctx;

typedef struct
{   aesstate rk[13];
} aes192ctx;

typedef struct
{   aesstate rk[15];
} aes256ctx;

typedef struct
{   aesstate *rounds;
    int keysize;
    int nkeywords;
    int nrounds;
    const uint8_t* key;
} deosaes;

typedef struct
{   int keysize;
    const char* key;
    const char* plain;
    const char* cipher;
} testdeosaes;

extern deosaes *newdeosaes(int keysize, const uint8_t* key);
extern int deldeosaes(deosaes *self);
extern void deosaesencrypt(deosaes *self,
                           size_t blocks,
                           unsigned char *ciphertext,
                           const unsigned char *plaintext);
extern void deosaesdecrypt(deosaes *self,
                           size_t blocks,
                           unsigned char *plaintext,
                           const unsigned char *ciphertext);
extern void deosaesfromhex(unsigned char* data,
                           int len,
                           const char* hex);

#ifdef __cplusplus
}
#endif /*cplusplus*/

#endif /*deoscryptoaes*/
