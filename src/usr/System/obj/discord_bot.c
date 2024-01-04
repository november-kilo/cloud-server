#include <kernel/user.h>
#include <kernel/kernel.h>
#include <Iterator.h>

inherit "~System/lib/discord_bot";

int login(string str) {
    if (previous_program() == LIB_CONN && str == "discord_bot") {
        ::connection(previous_object());
        DRIVER->message("Bot logged in from " + ip_number() + "\n");
        return MODE_NOCHANGE;
    }
}

int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        object *users;
        int i;
        IntIterator iter;

        users = users();
        i = sizeof(users);
        if (i > 0) {
            iter = new IntIterator(0, i - 1);
            while (!iter->end()) {
                users[iter->next()]->message("[Discord] " + str + "\n");
            }
        }

        return MODE_NOCHANGE;
    }
}

int message_done(void) {
    if (previous_program() == LIB_CONN) {
        ::message_done();
        return MODE_NOCHANGE;
    }
}

void logout(void) {
    if (previous_program() == LIB_CONN) {
        ::logout();
        destruct_object(this_object());
    }
}

void send(string str) {
    if (previous_program() == "/usr/System/obj/user") {
        message("::" + str + "\n");
    }
}
