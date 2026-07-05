#include <Number.h>

private float real_part;
private float imaginary_part;

float real() {
    return real_part;
}

float imag() {
    return imaginary_part;
}

string to_string() {
    return "" + real_part + " + " + imaginary_part + "i";
}

private void instance_of_bad_type(mixed x) {
    error("Complex: cannot operate with value of type " + typeof(x));
}

private Complex from(mixed x) {
    float f;

    switch (typeof(x)) {
        case T_FLOAT:
            return new Complex(x, 0.0);
        case T_INT:
            return new Complex((float) x, 0.0);
        case T_OBJECT:
            if (function_object("to_float", x)) {
                return new Complex(x->to_float(), 0.0);
            }
            if (x <- Complex) {
                return new Complex(x->real(), x->imag());
            }
    }

    if (catch(f = (float) x)) {
        instance_of_bad_type(x);
    }

    return new Complex(f, 0.0);
}

static Complex operator + (mixed that) {
    Complex z;
    float r, i;

    z = from(that);
    r = real_part + z->real();
    i = imaginary_part + z->imag();

    return new Complex(r, i);
}

static Complex operator - (mixed that) {
    Complex z;
    float r, i;

    z = from(that);
    r = real_part - z->real();
    i = imaginary_part - z->imag();

    return new Complex(r, i);
}

static Complex operator * (mixed that) {
    Complex z;
    float r, i;

    z = from(that);
    r = real_part * that->real() - imaginary_part * that->imag();
    i = imaginary_part * that->real() + real_part * that->imag();

    return new Complex(r, i);
}

static Complex operator / (mixed that) {
    Complex z;
    float cr, ci, r, i, denominator;

    if (fabs(that->real()) >= fabs(that->imag())) {
        r = that->imag() / that->real();
        denominator = that->real() + r * that->imag();
        cr = (real_part + r * imaginary_part) / denominator;
        ci = (imaginary_part - r * real_part) / denominator;
    } else {
        r = that->real() / that->imag();
        denominator = that->imag() + r * that->real();
        cr = (real_part * r + imaginary_part) / denominator;
        ci = (imaginary_part * r - real_part) / denominator;
    }

    return new Complex(cr, ci);
}

static void create(mixed r, mixed i) {
    ::real_part = (float) r;
    ::imaginary_part = (float) i;
}
