#ifndef STRING_UTILS_H

#define STRING_UTILS_H

#include <stdlib.h>
#include <string.h>
#include "sds.h"
#include "sdsalloc.h"
#include "lpc_ext.h"

sds lpcArrayToSds(LPC_array);
char *copyBlock(sds, int);

#endif
