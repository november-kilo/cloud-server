#include <Function.h>

inherit Function;

string evaluate(object user) {
    string user_name;

    user_name = user->query_name();
    if (!user_name) {
        user_name = "<unknown>";
    }
    return user_name;
}
