#ifndef __DEOSOCL__
#define __DEOSOCL__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ATDLIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /*ATDLIB*/

typedef struct deosOpenCLObject
{   int id;
} deosOpenCL;

#ifdef __cplusplus
}
#endif

#endif /*DEOSOCL*/
