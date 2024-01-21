#include <kernel/user.h>
#include <kernel/kernel.h>

inherit "~System/discord/lib/discord_bot";

int login(string str) {
    if (previous_program() == LIB_CONN && str == "discord_bot") {
        ::connection(previous_object());
        DRIVER->message("Discord bot logged in from " + ip_number() + "\n");
        return MODE_NOCHANGE;
    }
}
