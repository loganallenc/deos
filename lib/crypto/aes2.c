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

static deosaes *initdeosaes(int keysize, const uint8_t* key);

deosaes *newdeosaes(int keysize, const uint8_t* key)
{
    deosaes *self = initdeosaes(keysize, key);
    if (self->keysize == 128)
    {   aesstate rcon = {{1,0,0,0,0,0,0,0}};
        int pos = 0;
        int i;
        for (i=0; i<self->nrounds+1; i++)
        {   int b;
            for (b=0; b<8; b++) self->rounds[i].slice[b]=0;
        }
    }
    return self;
}

static deosaes *initdeosaes(int keysize, const uint8_t* key)
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

#ifdef __cplusplus
}
#endif
