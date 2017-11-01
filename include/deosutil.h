#ifndef __DEOSUTIL__
#define __DEOSUTIL__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ATDLIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /*ATDLIB*/

typedef struct deosUtilObject
{   int id;
} deosUtil;

extern void deosutilquine(void);

#ifdef __cplusplus
}
#endif

#endif /*DEOSUTIL*/
