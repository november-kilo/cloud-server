#include "over.h"

inherit Terminal;
inherit "/lib/util/string";
inherit tree "~over/lib/tree";
inherit exponential "~over/lib/exponential";

private object kant;

static void create(void) {
    kant = find_object(KANTLIPSUM);
    if (!kant) {
        kant = compile_object(KANTLIPSUM);
    }
}

string getPrompt(object user) {
    return pointer() + " ";
}
