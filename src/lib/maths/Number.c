#include <Maths.h>

private float number;

int toInt(void) {
    return (int) number;
}

float toFloat(void) {
    return number;
}

string toString(void) {
    return (string) number;
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

        if (!(that <- Number)) {
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

Number abs(void) {
    return new Number(fabs(number));
}

private float makeFloatFrom(mixed value) {
    float f;

    if (typeof(value) == T_OBJECT && value <- Number) {
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

static Number operator- (varargs mixed that) {
    if (that == nil) {
        return new Number(-number);
    }

    return new Number(number - makeFloatFrom(that));
}

static Number operator* (mixed that) {
    return new Number(number * makeFloatFrom(that));
}

static Number operator/ (mixed that) {
    float f;

    f = makeFloatFrom(that);
    if (f == 0.) {
        error("Number: division by zero.");
    }

    return new Number(number / f);
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
    int type;

    type = typeof(number);

    switch (type) {
        case T_INT:
        case T_FLOAT:
        case T_STRING:
            catch {
                ::number = (float) number;
                return;
            }
            break;
        case T_OBJECT:
            if (number <- Number) {
                ::number = number->toFloat();
                return;
            }
    }

    error("Number: invalid create argument.");
}
