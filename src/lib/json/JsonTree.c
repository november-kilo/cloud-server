#include <Iterator.h>
#include <Terminal.h>

inherit Terminal;

private string *lines;
private int indent;

static void create(void) {
    lines = ({ });
    indent = 0;
}

private string pipes(varargs int useAscii) {
    return useAscii ? "`-" : lpipe() + hpipe();
}

private string indentString(string str, int indent, varargs int useAscii) {
    return (indent > 0 ? spaces(3 * indent) : "") + pipes(useAscii);
}

void traverse(mapping *jsonParsedToMapping, varargs int useAscii) {
    Iterator iterator;
    string *keys;

    iterator = new IntIterator(0, sizeof(jsonParsedToMapping) - 1);
    while (!iterator->end()) {
        string str;
        int i;

        str = "";
        i = iterator->next();

        str += indentString(str, indent, useAscii);

        lines += ({ str + " " + jsonParsedToMapping[i]["key"] });

        if (jsonParsedToMapping[iterator->current()]["children"]) {
            indent++;
            traverse(jsonParsedToMapping[i]["children"], useAscii);
        }

        if (i == sizeof(jsonParsedToMapping) - 1) {
            --indent;
        }
    }
}

string toString(void) {
    return implode(lines, "\n");
}
