#include <Iterator.h>
#include <Terminal.h>

inherit Terminal;
inherit "/lib/util/string";

private string *out;
private int indent;

static void create(void) {
    out = ({ });
    indent = 0;
}

string toString(void) {
    return implode(out, "\n");
}

private string pipes(varargs int useAscii) {
    return useAscii ? "`-" : lpipe() + hpipe();
}

void traverse(mapping *tree, varargs int useAscii) {
    Iterator iterator;
    string *keys;

    iterator = new IntIterator(0, sizeof(tree) - 1);
    while (!iterator->end()) {
        string str;
        int i;

        str = "";
        i = iterator->next();

        if (indent == 0) {
            str += pipes(useAscii);
        } else {
            str += spaces((3 * indent)) + pipes(useAscii);
        }
        out += ({ str + " " + tree[i]["key"] });
        if (tree[iterator->current()]["children"]) {
            indent++;
            traverse(tree[i]["children"]);
        }
        if (i == sizeof(tree) - 1) {
            --indent;
        }
    }
}

