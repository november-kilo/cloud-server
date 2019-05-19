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

int equals(mixed that, varargs float tolerance) {
    int thatType;
    float f1, f2, d;

    if (number == that) {
        return TRUE;
    }

    thatType = typeof(that);
    if (thatType == T_OBJECT) {
        if (this_object() == that) {
            return TRUE;
        }

        if (!(that <- NUMBER_LIB)) {
            return FALSE;
        }

        f2 = that->toFloat();
    } else {
        if (catch(f2 = (float) that)) {
            return FALSE;
        }
    }

    f1 = toFloat();
    d = fabs(f1 - f2);

    return d == 0. || d < tolerance;
}

private float makeFloatFrom(mixed value) {
    float f;

    if (typeof(value) == T_OBJECT && value <- NUMBER_LIB) {
        return value->toFloat();
    }

    if (catch(f = (float) value)) {
        error("Number: invalid operand.");
    }

    return f;
}

static Number operator+ (mixed that) {
    return new Number(number + makeFloatFrom(that));
}

static Number operator- (mixed that) {
    return new Number(number - makeFloatFrom(that));
}

static Number operator* (mixed that) {
    return new Number(number * makeFloatFrom(that));
}

static Number operator/ (mixed that) {
    return new Number(number / makeFloatFrom(that));
}

static int operator< (mixed that) {
    return number < makeFloatFrom(that);
}

static int operator<= (mixed that) {
    return number <= makeFloatFrom(that);
}

static int operator> (mixed that) {
    return number > makeFloatFrom(that);
}

static int operator>= (mixed that) {
    return number >= makeFloatFrom(that);
}

static void create(mixed number) {
    ::number = number;
}
