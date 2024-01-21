#include <kernel/kernel.h>
#include <kernel/user.h>

inherit "~System/lib/user";

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
    return prev == "/usr/System/obj/user" || sscanf(prev, "/usr/%*s/obj/wiztool");
}

int send(string str, varargs string prefix) {
    if (is_valid(previous_program())) {
        message((prefix == nil ? "" : prefix) + str + "\n");
        return 1;
    }

    return 0;
}
