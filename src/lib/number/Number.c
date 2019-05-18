#include <NKlib.h>

private mixed number;

int toInt(void) {
    return (int) number;
}

float toFloat(void) {
    return (float) number;
}

Rational toRational(void) {
    return new Rational(toFloat());
}

string toString(void) {
    return (string) number;
}

int isNan() {
    int t;

    t = typeof(number);
    switch (t) {
        case T_OBJECT:
            return number <- NUMBER_LIB ? FALSE : TRUE;
        case T_INT:
        case T_FLOAT:
            return FALSE;
    }

    return TRUE;
}

static void create(mixed number) {
    ::number = number;
}
