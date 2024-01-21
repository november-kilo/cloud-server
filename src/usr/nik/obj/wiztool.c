#include <kfun.h>

inherit terminal "/lib/util/terminal";
inherit format "/lib/util/format";

private object me;

static void create(void) {
    me = "/usr/System/sys/userd"->find_user("nik");
}

private int valid(string str) {
    return str == "/usr/System/obj/user";
}

void clear_screen(void) {
    if (valid(previous_program())) {
        me->message(terminal::clear());
    }
}

string get_prompt(void) {
    if (valid(previous_program())) {
        string name;
        string cwd;
        string end;
        string prompt;

        name = terminal::cyan(me->query_name(), 1);
        cwd = terminal::green(me->query_directory());
        end = terminal::arrowRight();

        prompt = format::format("%s:%s %s ", ({ name, cwd, end }));

        if (prompt != nil) {
            return prompt;
        }
    }

    return "> ";
}

void cmd_cls(string arg) {
    if (valid(previous_program())) {
        me->message(terminal::clear());
    }
}

void cmd_disco(string str) {
    if (valid(previous_program())) {
        object discord_bot;

        discord_bot = "/usr/System/discord/sys/discord_botd"->get_discord_bot();
        if (discord_bot == nil) {
            me->println("The Discord bot is not connected.");
            return;
        }
        if (discord_bot->send(str)) {
            me->println("Message sent to Discord bot.");
        }
    }
}

void cmd_pi(string str) {
    if (valid(previous_program())) {
        string template, pi, prefix;
        int i;

        sscanf(str, "%d", i);
        template = "%." + i + "LF";
        prefix = format::sprintf("%26s", "pi =");
        pi = format::sprintf(template, 4.0 * atan(1.0));
        me->println(format::format("%s %s", ({ prefix, pi })));
    }
}
