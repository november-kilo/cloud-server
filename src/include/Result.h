#ifndef RESULT_LIB_H

#define RESULT_LIB_H

#define RESULT_LIB "/lib/Result"
#define Result object RESULT_LIB

#define RESULT_SUCCESS(v) (new Result(1, v, nil))
#define RESULT_FAILURE(e) (new Result(0, nil, e))

#endif
