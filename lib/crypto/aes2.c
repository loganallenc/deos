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

deosaes *newdeosaes(int keysize)
{
    deosaes *self = initdeosaes();

    if (keysize == 128 || keysize == 192 || keysize == 256)
        self->keysize = keysize;

    return self;
}

deosaes *initdeosaes(void)
{
    deosaes *self = (deosaes *) malloc(sizeof(deosaes));
    if (self == NULL) return NULL;
    memset(self, 0, sizeof(deosaes));
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
