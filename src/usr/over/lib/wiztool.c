#include "over.h"

inherit Terminal;
inherit "/lib/util/string";

private object kant;

static void create(void) {
    kant = find_object(KANTLIPSUM);
    if (!kant) {
        kant = compile_object(KANTLIPSUM);
    }
}

string getPrompt(object user) {
    return pointer() + " ";
}

private int isValidCall(string caller) {
    return caller == "/usr/System/obj/user";
}

private string absPath(object user, string str) {
    return DRIVER->normalize_path(str, user->query_directory(), query_owner());
}

void cmd_vector(object user, string cmd, string str) {
    Vector a, b, c;
    mapping cylindrical, polar, spherical;

    if (!isValidCall(previous_program())) {
        return;
    }

    a = new Vector(({ new Number(3), new Number(5), new Number(7) }));
    b = new Vector(({ new Number(4), new Number(6), new Number(8) }));

    c = a->cross(b);
    user->println("A x B = " + c->toString());

    cylindrical = a->cylindrical();
    user->println("A in cylindrical:\n\tradial: " + cylindrical["radial"]->toString() + "\n" +
                  "\tazimuthal: " + cylindrical["azimuthal"]["deg"]->toString() + " deg\n" +
                  "\tvertical: " + cylindrical["vertical"]->toString());

    polar = a->polar();
    user->println("A in polar:\n\tradial: " + polar["radial"]->toString() + "\n" +
                  "\tangular: " + polar["angular"]["deg"]->toString() + " deg");

    spherical = a->spherical();
    user->println("A in spherical:\n\tradial: " + spherical["radial"]->toString() + "\n" +
                  "\tpolar: " + spherical["polar"]["deg"]->toString() + " deg\n" +
                  "\tazimuthal: " + spherical["azimuthal"]["deg"]->toString() + " deg");
}

void cmd_regex(object user, string cmd, string str) {
    mapping results;

    if (!isValidCall(previous_program())) {
        return;
    }

    user->println("find first...");
    results = regexp(str, FALSE);
    if (results["error"]) {
        user->println("regex: " + results["error"]);
        return;
    }

    user->println(dump_value(results, ([])));

    user->println("find global...");
    results = regexp(str, TRUE);
    if (results["error"]) {
        user->println("regex: " + results["error"]);
        return;
    }

    user->println(dump_value(results, ([])));
}

void cmd_match(object user, string cmd, string str) {
    string *words, *results;
    string errorString;
    int i, sz;

    if (!isValidCall(previous_program())) {
        return;
    }

    if (!str || str == "") {
        user->println("usage: match <words>");
        return;
    }

    words = explode(str, " ");
    errorString = catch(results = kant->matches(words));
    if (errorString) {
        user->message("error: " + errorString);
        return;
    }
    if (sizeof(results) == 0) {
        user->println("matches: no words found");
        return;
    }
    user->println(break_string(implode(results, "\n\n"), 120, 0));
    user->println("found " + sizeof(results));
}

static string *fetchAristotle(void) {
    object kant;

    kant = find_object(KANTLIPSUM);
    if (!kant) {
        kant = compile_object(KANTLIPSUM);
    }
    return kant->matches(({ "Aristotle" }));
}

static int transformAristotle(string *aristotle, object user) {
    int sz;
    string str, thePhilosopher;
    Iterator iterator;

    sz = sizeof(aristotle);
    if (sz) {
        thePhilosopher = new Terminal->bold("The Philosopher");
        iterator = new IntIterator(0, sz - 1);
        while (!iterator->end()) {
            aristotle[iterator->next()] = perl_sub(
                    aristotle[iterator->current()],
                    "s/Aristotle/" + thePhilosopher + "/g"
            );
        }
        str = implode(aristotle, "\n\n");
        user->println(break_string(str, 120, 0));
    } else {
        user->println("no me gusta");
    }

    user->showPrompt();
    return 1;
}

void cmd_aristotle(object user, string cmd, string str) {
    Continuation c;

    if (!isValidCall(previous_program())) {
        return;
    }

    c = new Continuation("fetchAristotle");
    c >>= new ChainedContinuation("transformAristotle", user);
    c->runNext();
}

void cmd_lsr(object user, string cmd, string str) {
    Iterator iterator;
    FileTree fileTree;
    string pre, post, path, owner, file;
    mixed *finfo;

    if (!isValidCall(previous_program())) {
        return;
    }

    if (!str || str == "") {
        user->println("usage: lsr <path>");
        return;
    }

    owner = query_owner();
    path = absPath(user, str);
    iterator = new Iterator(new FileTree(path), nil, nil);
    iterator->next();
    while (!iterator->end()) {
        file = iterator->next();
        finfo = file_info(file);
        pre += ctime(finfo[1]);

        if (finfo[2] || finfo[2] == 1) {
            post = "*";
        } else {
            post = "";
        }

        if (path == "/") {
            file = file[1..];
        } else {
            file = file[strlen(path) + 1..];
        }

        user->println(pre + " " + file + post);
    }
}
