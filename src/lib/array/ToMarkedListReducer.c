#include <Array.h>

inherit ArrayToListReducer;

#define LIST_GAP " "

private int order;

private string fmtVal(mixed first, mixed second) {
    return toString(first) +
        (order > 0 ? (order++) + ")" : pointer()) +
        LIST_GAP +
        toString(second) + "\n";
}

static string ifNotNextValue(mixed currentValue, varargs int isLast) {
    return fmtVal("", currentValue);
}

static string ifNextValue(mixed nextValue, mixed currentValue, varargs int isLast) {
    return fmtVal(currentValue, nextValue);
}

void apply(int order) {
    ::order = order;
}

static void create(varargs int order) {
    ::order = order;
}