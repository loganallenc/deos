#include "deostx.h"

#ifdef __cplusplus
extern "C" {
#endif

deostx *newdeostx(void)
{
    deostx *self = initdeostx();
    self->i = 14;
    return self;
}

deostx *initdeostx(void)
{
    deostx *self = (deostx *) malloc(sizeof(deostx));
    if (self == NULL) return NULL;
    memset(self, 0, sizeof(deostx));
    return self;
}

int deldeostx(deostx *self)
{
    if (NULL == self) return -1;
    free(self);
    return 0;
}

#ifdef __cplusplus
}
#endif
