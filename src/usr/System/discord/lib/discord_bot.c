#include <kernel/user.h>
#include <kernel/kernel.h>
#include <Iterator.h>

inherit "~System/lib/bot";

static int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        object *users;
        int i;
        IntIterator iter;

        users = users();
        i = sizeof(users);
        if (i > 0) {
            iter = new IntIterator(0, i - 1);
            while (!iter->end()) {
                object user;

                user = users[iter->next()];
                user->println("[Discord] " + str);
                user->show_prompt();
            }
        }

        return MODE_NOCHANGE;
    }
}
