#include "~/include/Test.h"

static void create(void) {
    compile_object(MOCK_MILLITIME_LIB);
    compile_object(MOCK_RANDOM_LIB);
    compile_object(MOCK_TIME_LIB);
    compile_object(TEST_FUNCTION);
    compile_object(TEST_LIB);
    compile_object(TEST_RUNNER);
}