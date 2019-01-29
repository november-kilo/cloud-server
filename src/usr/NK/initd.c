#include <NKlib.h>

static void create(void) {
    compile_object(DICE_EXPERIMENT_OBJ);
    compile_object(MOCK_RANDOM_LIB);
    compile_object(TEST_FUNCTION);
    compile_object(TEST_RUNNER);
    compile_object("opt/cmds");
}
