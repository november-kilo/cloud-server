#include "nik.h"

string getPrompt(object user) {
    return user->query_name() + "> ";
}
