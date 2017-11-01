#ifndef __DEOSADDR__
#define __DEOSADDR__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ATDLIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /*ATDLIB*/

typedef struct deosAddrObject
{   int id;
} deosAddr;

#ifdef __cplusplus
}
#endif

#endif /*DEOSADDR*/
