#include <kernel/user.h>

inherit "~System/lib/bot";

static int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        mapping map;
        object user;

        map = "/sys/jsondecode"->decode(str);
        user = "/usr/System/sys/userd"->find_user(map["name"]);

        if (!user) {
            return MODE_NOCHANGE;
        }

        user->println("[Discord] " + map["message"]);
        user->show_prompt();

        return MODE_NOCHANGE;
    }
}
