#include <kfun.h>
#include <Array.h>
#include <Iterator.h>

inherit terminal "/lib/util/terminal";
inherit format "/lib/util/format";
inherit base64 "/lib/util/base64";

#define JSONENCODE	"/sys/jsonencode"

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

private string base64_encode(string str) {
    return base64::encode(str);
}

private string json_encode(mapping map) {
    return JSONENCODE->encode(map);
}

private string encode_discord_message(string str) {
    mapping map;
    map = ([]);
    map["name"] = me->query_name();
    map["command"] = str;
    return json_encode(map);
}

private int starts_with(string a, string b) {
    if (strlen(a) < strlen(b)) {
        return FALSE;
    }

    return a[0..strlen(b) - 1] == b;
}

private int is_command_known_to_discord(string str) {
    if (!str || str == "") {
        return FALSE;
    }

    return str == "mcstatus";
}

void cmd_disco(string str) {
    if (valid(previous_program())) {
        object discord_bot;

        if (!is_command_known_to_discord(str)) {
            me->println("Unknown Discord bot command.");
            return;
        }

        discord_bot = "/usr/System/discord/sys/discord_botd"->get_discord_bot();
        if (discord_bot == nil) {
            me->println("The Discord bot is not connected.");
            return;
        }

        if (discord_bot->send(encode_discord_message(str))) {
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

void cmd_colors(string str) {
    if (valid(previous_program())) {
        Iterator iterator;
        int *heatMap;

        heatMap = allocate_int(7);
        heatMap[0] = 46;
        heatMap[1] = 226;
        heatMap[2] = 220;
        heatMap[3] = 214;
        heatMap[4] = 208;
        heatMap[5] = 202;
        heatMap[6] = 196;
        iterator = new IntIterator(0, 6);

        while (!iterator->end()) {
            me->message(terminal::color256(heatMap[iterator->next()], "x"));
        }

        me->println("\nDone.");
    }
}

void cmd_arrays(string str) {
    if (valid(previous_program())) {
        Array a;
        a = new Array(({ 3, 1, 2 }));
        me->println(a->stringify());
        a->sort();
        me->println(a->stringify());
    }
}
