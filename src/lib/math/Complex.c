#include <Math.h>

inherit Number;

private Number r;
private Number i;

static void create(mixed r, mixed i, varargs int polarFlag) {
    r = new Number(r);
    i = new Number(i);

    ::r = polarFlag ? r->toFloat() * cos(i->toFloat()) : r;
    ::i = polarFlag ? r->toFloat() * sin(i->toFloat()) : i;
}

Number re(void) {
    return r;
}

Number im(void) {
    return i;
}

string toString(void) {
    return "(" + r->toString() + " + (" + i->toString() + "i))";
}

Number arg(void) {
    Number x, y;

    x = re();
    y = im();

    if (x->isZero() && y->isZero()) {
        return new Number(0);
    }

    return y->_atan2(x);
}

Number abs(void) {
    return new Number(hypot(re()->toFloat(), im()->toFloat()));
}

Number absSquared(void) {
    return re() * re() + im() * im();
}

Number logAbs(void) {
#ifndef KF_LOG1P
    error("Complex->logAbs() is unavailable");
#endif

    Number xa, ya, max, u;

    xa = re()->abs();
    ya = im()->abs();

    if (xa >= ya) {
        max = xa;
        u = ya / xa;
    } else {
        max = ya;
        u = xa / ya;
    }

    return max->_log() + new Number((log1p(u->toFloat() * u->toFloat()) / 2.0));
}

Complex conj(void) {
    return new Complex(re(), -im());
}

Complex negate(void) {
    return new Complex(-re(), -im());
}

Complex reciprocal(void) {
    Number r, i, x;

    x = new Number(1.0) / this_object()->abs();
    r = (re() * x) * x;
    i = -(im() * x) * x;

    return new Complex(r, i);
}

Complex inverse(void) {
    Number s;

    s = new Number(1) / abs();

    return new Complex((re() * s) * s, -(im() * s) * s);
}

Complex sqrt(void) {
    float x, y, t, w, ai, vi;

    if (re()->isZero() && im()->isZero()) {
        return new Complex(0, 0);
    }

    x = re()->abs()->toFloat();
    y = im()->abs()->toFloat();

    if (x >= y) {
        t = (y / x);
        w = ::sqrt(x) * ::sqrt((1.0 + ::sqrt(1.0 + t * t)) / 2.0);
    } else {
        t = (x / y);
        w = ::sqrt(y) * ::sqrt((t + ::sqrt(1.0 + t * t)) / 2.0);
    }

    if (re() >= 0) {
        ai = im()->toFloat();
        return new Complex(w, ai / (2.0 * w));
    }

    ai = im()->toFloat();
    vi = (ai >= 0.0) ? w : -w;

    return new Complex(ai / (2.0 * vi), vi);
}

Complex exp(void) {
    Number x, y;

    x = re()->_exp();
    y = im();

    return new Complex(x * y->_cos(), x * y->_sin());
}

private Complex powComplex(Complex that) {
    Number logr, br, bi, beta, theta, rho;

    if (re()->isZero() && im()->isZero()) {
        if (that->re()->isZero() && that->im()->isZero()) {
            error("Complex: result undefined");
        }
        return new Complex(0, 0);
    } else if (that->re()->toFloat() == 1.0 && that->im()->isZero()) {
        return new Complex(re(), im());
    } else if (that->re()->toFloat() == -1.0 && that->im()->isZero()) {
        return inverse();
    }

    logr = logAbs();
    theta = arg();
    br = that->re();
    bi = that->im();
    rho = (logr * br - bi * theta)->_exp();
    beta = theta * br + bi * logr;

    return new Complex(rho * beta->_cos(), rho * beta->_sin());
}

private Complex powReal(Number that) {
    Number logr, theta, rho, beta;

    if (re()->isZero() && im()->isZero()) {
        if (that->isZero()) {
            error("Complex: result undefined");
        }
        return new Complex(0, 0);
    }

    logr = logAbs();
    theta = arg();
    rho = (logr * that)->_exp();
    beta = theta * that;

    return new Complex(rho * beta->_cos(), rho * beta->_sin());
}

Complex pow(mixed that) {
    if (typeof(that) == T_OBJECT && that <- Complex) {
        return powComplex(that);
    }

    return powReal(new Number(that));
}

Complex log(void) {
    return new Complex(logAbs(), arg());
}

Complex log10(void) {
    Complex l;

    return log() * (1.0 / ::log(10.0));
}

private Number *operands(Complex this, mixed that) {
    Number xr, xi, yr, yi;

    xr = re();
    xi = im();

    if (typeof(that) == T_OBJECT && that <- Complex) {
        yr = that->re();
        yi = that->im();
    } else {
        yr = new Number(that);
        yi = new Number(0);
    }

    return ({ xr, xi, yr, yi });
}

static Complex operator+ (mixed that) {
    Number xr, xi, yr, yi;

    ({ xr, xi, yr, yi }) = operands(this_object(), that);

    return new Complex(xr + yr, xi + yi);
}

static Complex operator- (mixed that) {
    Number xr, xi, yr, yi;

    ({ xr, xi, yr, yi }) = operands(this_object(), that);

    return new Complex(xr - yr, xi - yi);
}

static Complex operator* (mixed that) {
    Number xr, xi, yr, yi;

    ({ xr, xi, yr, yi }) = operands(this_object(), that);

    return new Complex(xr * yr - xi * yi, xr * yi + xi * yr);
}

static Complex operator/ (mixed that) {
    Number xr, xi, yr, yi;
    Number r, d;
    Number zr, zi;

    xr = re();
    xi = im();

    if (typeof(that) == T_OBJECT && that <- Complex) {
        yr = that->re();
        yi = that->im();

        if (yr->abs() > yi->abs()) {
            r = yi / yr;
            d = yr + r * yi;
            zr = (xr + r * xi) / d;
            zi = (xi - r * xr) / d;
        } else {
            r = yr / yi;
            d = yi + r * yr;
            zr = (r * xr + xi) / d;
            zi = (r * xi - xr) / d;
        }
    } else {
        r = new Number(that);
        zr = xr / r;
        zi = xi / r;
    }

    return new Complex(zr, zi);
}

static Complex operator^ (mixed that) {
    return pow(that);
}
