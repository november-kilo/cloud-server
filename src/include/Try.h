#ifndef TRY_LIB_H

#define TRY_LIB_H

#include <Result.h>

#define TRY_LIB "/lib/Try"
#define Try object TRY_LIB

#define TRY_SUCCESS(v) (new Try(RESULT_SUCCESS(v)))
#define TRY_FAILURE(e) (new Try(RESULT_FAILURE(e)))

#endif
