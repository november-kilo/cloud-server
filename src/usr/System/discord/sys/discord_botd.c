#include <kernel/user.h>

inherit "~/lib/user";

object discord_bot;
object userd;

static void create(void) {
    userd = find_object(USERD);
    userd->set_binary_manager(1, this_object());
}

object get_discord_bot(void) {
    return discord_bot;
}

object select(string name) {
    if (previous_object() == userd && name == "discord_bot") {
        discord_bot = clone_object("~System/discord/obj/discord_bot");
        return discord_bot;
    }
}

int query_mode(object obj) {
    return MODE_NOECHO;
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
