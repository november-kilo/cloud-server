#include <nk_lib.h>

private void load_objects(string *paths) {
    int i, sz;

    for (i = 0, sz = sizeof(paths); i < sz; i++) {
        call_other(compile_object(paths[i]), "???");
    }
}

private void compile_libs(string *libs) {
    int i, sz;

    for (i = 0, sz = sizeof(libs); i < sz; i++) {
        compile_object(libs[i]);
    }
}

static void create(void) {
    string *libs;

    libs = ({
        GREATER_THAN_PREDICATE,
        NAME_LOCATION_KEY_EXTRACTOR
    });
    compile_libs(libs);
}
