#include "over.h"

inherit Terminal;
inherit "/lib/util/string";

string getPrompt(object user) {
    return pointer() + " ";
}
