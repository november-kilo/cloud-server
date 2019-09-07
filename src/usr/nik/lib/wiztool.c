#include "nik.h"

inherit Terminal;

string getPrompt(object user) {
    return magenta(user->query_name() + "> ", TRUE);
}
