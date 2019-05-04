#include <NKlib.h>

inherit NK_KFUN;

private float re;
private float im;

float re(void) {
    return re;
}

float im(void) {
    return im;
}

string toString(void) {
    return "(" + re + " + " + im + " i)";
}

Complex conjugate(void) {
    return new Complex(re, -im);
}

static Complex operator/ (Complex y) {
    float xr, xi, yr, yi, r, d;

    xr = re();
    xi = im();
    yr = y->re();
    yi = y->im();

    if (yr == 0. && yi == 0.) {
        error("Complex: division by zero.");
    }

    if (fabs(yr) > fabs(yi)) {
        r = yi / yr;
        d = yr + r * yi;
        return new Complex((xr + r * xi) / d, (xi - r * xr) / d);
    }

    r = yr / yi;
    d = yi + r * yr;
    return new Complex((r * xr + xi) / d, (r * xi - xr) / d);
}

static void create(float x, float y, varargs int fromPolar) {
    if (fromPolar) {
        if (x < 0.) {
            x = -x;
            y += pi();
        }
        create(x * cos(y), x * sin(fmod(y, 2. * pi())));
    } else {
        re = x;
        im = y;
    }
}