#include <Number.h>

private float number;

static void create(mixed number) {
    switch (typeof(number)) {
        case T_FLOAT:
            ::number = number;
            return;

        case T_INT:
        case T_STRING:
            catch {
                ::number = (float) number;
                return;
            }
            break;

        case T_OBJECT:
            if (function_object("to_float", number)) {
                ::number = number->to_float();
                return;
            }
    }

    error("Number: invalid create argument");
}

static float float_from(mixed value) {
    float f;
    int has_to_float;

    has_to_float = typeof(value) == T_OBJECT && (function_object("to_float", value) || value <- Number);
    if (has_to_float) {
        return value->to_float();
    }

    if (catch(f = (float) value)) {
        error("Number: invalid operand");
    }

    return f;
}

float to_float(void) {
    return number;
}

int to_int(void) {
    return (int) number;
}

int is_infinitesimal(void) {
    return fabs(number) < FLT_EPSILON;
}

int compare(Number that, varargs float epsilon) {
    float delta, difference, a, b, fraction_part;
    int exponent;

    if (!epsilon) {
        epsilon = TOLERANCE;
    }

    a = ::number;
    b = float_from(that);
    ({ fraction_part, exponent }) = frexp(((fabs(a) > fabs(b)) ? a : b));
    delta = ldexp(epsilon, exponent);
    difference = a - b;

    if (difference > delta) {
        return THIS_GT_THAT;
    }

    if (difference < -delta) {
        return THIS_LT_THAT;
    }

    return THIS_EQ_THAT;
}

private int outside(float a, float b) {
    return number < a || number > b;
}

Number abs(void) {
    return new Number(fabs(number));
}

Number acos(void) {
    if (outside(-1.0, 1.0)) {
        error("Number: invalid argument for acos: " + number);
    }
    return new Number(::acos(number));
}

Number asin(void) {
    if (outside(-1.0, 1.0)) {
        error("Number: invalid argument for asin: " + number);
    }
    return new Number(::asin(number));
}

Number atan(void) {
    return new Number(::atan(number));
}

Number atan2(Number x) {
    return new Number(::atan2(number, x->to_float()));
}

Number ceil(void) {
    return new Number(::ceil(number));
}

Number cos(void) {
    return new Number(::cos(number));
}

Number cosh(void) {
    return new Number(::cosh(number));
}

Number exp(void) {
    return new Number(::exp(number));
}

Number floor(void) {
    return new Number(::floor(number));
}

Number fmod(Number y) {
    return new Number(::fmod(number, y->to_float()));
}

Number* frexp(void) {
    float f;
    int n;

    ({ f, n }) = ::frexp(number);

    return ({ new Number(f), new Number(n) });
}

Number hypot(Number b) {
    return new Number(::hypot(number, b->to_float()));
}

Number ldexp(int n) {
    return new Number(::ldexp(number, n));
}

Number log(void) {
    return new Number(::log(number));
}

Number log10(void) {
    return new Number(::log10(number));
}

Number log1p(void) {
    return new Number(::log1p(number));
}

Number* modf(void) {
    float f, n;

    ({ f, n }) = ::modf(number);

    return ({ new Number(f), new Number(n) });
}

Number pow(Number y) {
    return new Number(::pow(number, y->to_float()));
}

Number random(Number mod) {
    return new Number(::random(mod->to_int()));
}

Number sin(void) {
    return new Number(::sin(number));
}

Number sinh(void) {
    return new Number(::sinh(number));
}

Number sqrt(void) {
    return new Number(::sqrt(number));
}

Number tan(void) {
    return new Number(::tan(number));
}

Number tanh(void) {
    return new Number(::tanh(number));
}

static int operator < (mixed that) {
    return this_object()->compare(that) == THIS_LT_THAT;
}

static int operator <= (mixed that) {
return this_object()->compare(that) != THIS_GT_THAT;
}

static int operator > (mixed that) {
    return this_object()->compare(that) == THIS_GT_THAT;
}

static int operator >= (mixed that) {
    return this_object()->compare(that) != THIS_LT_THAT;
}

static Number operator + (mixed that) {
    return new Number(number + float_from(that));
}

static Number operator- (varargs mixed that) {
    if (that == nil) {
        return new Number(-number);
    }

    return new Number(number - float_from(that));
}

static Number operator * (mixed that) {
    return new Number(number * float_from(that));
}

static Number operator / (mixed that) {
    float f;

    f = float_from(that);
    if (f == 0.0) {
        error("Number: division by zero");
    }

    return new Number(number / f);
}

static Number operator ^ (mixed that) {
    return pow(new Number(that));
}
