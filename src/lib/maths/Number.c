#include <Maths.h>

private float number;

int toInt(void) {
    return (int) number;
}

float toFloat(void) {
    return number;
}

string toString(void) {
    return ::toString(number);
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

Number _acos(void) {
    return new Number(acos(number));
}

Number _asin(void) {
    return new Number(asin(number));
}

Number _atan(void) {
    return new Number(atan(number));
}

Number _atan2(Number x) {
    return new Number(atan2(number, x->toFloat()));
}

Number _ceil(void) {
    return new Number(ceil(number));
}

Number _cos(void) {
    return new Number(cos(number));
}

Number _cosh(void) {
    return new Number(cosh(number));
}

Number _exp(void) {
    return new Number(exp(number));
}

Number _fabs(void) {
    return abs();
}

Number _floor(void) {
    return new Number(floor(number));
}

Number _fmod(Number y) {
    return new Number(fmod(number, y->toFloat()));
}

Number *_frexp(void) {
    float f;
    int n;

    ({ f, n }) = frexp(number);

    return ({ new Number(f), new Number(n) });
}

Number _hypot(Number b) {
    return new Number(hypot(number, b->toFloat()));
}

Number _ldexp(int n) {
    return new Number(ldexp(number, n));
}

Number _log(void) {
    return new Number(log(number));
}

Number _log10(void) {
    return new Number(log10(number));
}

Number *_modf(void) {
    float f, n;

    ({ f, n }) = modf(number);

    return ({ new Number(f), new Number(n) });
}

Number _pow(Number y) {
    return new Number(pow(number, y->toFloat()));
}

Number _random(Number mod) {
    return new Number(random(mod->toInt()));
}

Number _sin(void) {
    return new Number(sin(number));
}

Number _sinh(void) {
    return new Number(sinh(number));
}

Number _sqrt(void) {
    return new Number(sqrt(number));
}

Number _tan(void) {
    return new Number(tan(number));
}

Number _tanh(void) {
    return new Number(tanh(number));
}

private float makeFloatFrom(mixed value) {
    float f;

    if (typeof(value) == T_OBJECT && value <- Number) {
        return value->toFloat();
    }

    if (catch(f = (float) value)) {
        error("Number: invalid operand");
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
        error("Number: division by zero");
    }

    return new Number(number / f);
}

static Number operator^ (mixed that) {
    return _pow(that);
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

    error("Number: invalid create argument");
}
