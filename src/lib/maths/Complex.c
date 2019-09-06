#include <Maths.h>

private float r;
private float i;

static void create(float r, float i) {
    ::r = r;
    ::i = i;
}

float re(void) {
    return r;
}

float im(void) {
    return i;
}

string toString() {
    return "(" + r + " + (" + i + "i))";
}

static Complex operator/ (Complex that) {
    float xr, xi, yr, yi;
    float r, d;
    float zr, zi;

    xr = re();
    xi = im();
    yr = that->re();
    yi = that->im();

    if (fabs(yr) > fabs(yi)) {
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

    return new Complex(zr, zi);
}
