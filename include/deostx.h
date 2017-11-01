#ifndef __DEOSTX__
#define __DEOSTX__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ATDLIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /*ATDLIB*/

typedef struct deostxi
{   int i;
} deostx;

extern deostx *newdeostx(void);
extern deostx *initdeostx(void);
extern int deldeostx(deostx *self);

#ifdef __cplusplus
}
#endif

#endif /*DEOSTX*/
