#include <Iterator.h>
#include <Terminal.h>

inherit Terminal;
inherit "/lib/util/string";

private string *out;
private int indent;

void init(void) {
    out = ({ });
    indent = 0;
}

string toString(void) {
    return implode(out, "\n");
}

void traverse(mapping *tree) {
    Iterator iterator;
    string *keys;

    iterator = new IntIterator(0, sizeof(tree) - 1);
    while (!iterator->end()) {
        string str;
        int i;

        str = "";
        i = iterator->next();

        if (indent == 0) {
            str += lpipe() + hpipe();
        } else {
            str += spaces((3 * indent)) + lpipe() + hpipe();
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

/*
function walk(tree) {
    tree.forEach(function(node) {
        var str = '';
        if (indent < 2) {
            str = '└─';
        } else {
            for (let i = 2; i < indent * 2; i++) {
                str += ' ';
            }
            str += ' └─';
        }

        output.push(str + ' ' + node.key);
        if(node.children) {
            indent ++;
            walk(node.children);
        }
        if(tree.indexOf(node) === tree.length - 1) {
            indent--;
        }
    })
}
 */