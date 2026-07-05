/* Immutable Result type. */

#include <Result.h>

inherit format "/lib/util/format";

private int ok;
private mixed value;
private string err;

static void create(int ok, mixed value, string err) {
    ::ok = ok;
    ::value = value;
    ::err = err;
}

int ok() {
    return ok;
}

private void result_error(string str) {
    error("Result " + str + " is undefined.");
}

mixed value() {
    if (!ok) {
        result_error("value");
    }
    return value;
}

string err() {
    if (ok) {
        result_error("err");
    }
    return err;
}

string fold() {
    return ok
        ? format::str_format("Value: {:value}", ([ "value": value ]))
        : "Error: " + err;
}
