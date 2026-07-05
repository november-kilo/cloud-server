#include <kernel/kernel.h>
#include <kernel/user.h>
#include <Array.h>

inherit "~System/lib/user";
inherit format "/lib/util/format";

int query_mode(object obj) {
    return MODE_LINE;
}

int query_timeout(object obj) {
    return DEFAULT_TIMEOUT;
}

string query_banner(object obj) {
    return "stay awhile and listen\n";
}

int login(string str) {
    previous_object()->message("Access denied.\n\n");
    return MODE_DISCONNECT;
}

int receive_message(string str) {
    return MODE_NOCHANGE;
}

int is_valid(string prev) {
    Array objs;

    objs = new Array(({
        "/usr/System/obj/user",
        "/usr/System/discord/obj/discord_bot"
    }));
    return !!objs->find_first(prev)[1];
}

static void save_me(void) {}

int send(string str, varargs string prefix) {
    string msg;

    DRIVER->message("send: " + previous_program() + "\n");
    if (is_valid(previous_program())) {
        msg = (prefix == nil ? "" : prefix) + str;
        DRIVER->message("sending message: " + msg + "\n");
        message(msg + "\n");
        return 1;
    }

    return 0;
}
