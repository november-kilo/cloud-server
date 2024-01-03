#include <kernel/user.h>
#include <kernel/kernel.h>

inherit "~System/lib/discord_bot";

int login(string str) {
    if (previous_program() == LIB_CONN && str == "discord_bot") {
        ::connection(previous_object());
        DRIVER->message("Bot logged in.\n");
        return MODE_NOCHANGE;
    }
}

int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        DRIVER->message("Bot received: '" + str + "'\n");
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
