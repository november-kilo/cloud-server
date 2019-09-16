#include <Maths.h>

inherit Quadrature;

static float s;

float mpeval(float x) {
    return f->evaluate(x);
}

float next(void) {
    int it, j;
    float x, tnm, sum, del, ddel;

    n++;

    if (n == 1) {
        return (s = (b - a) * mpeval(0.5 * (a + b)));
    } else {
        for (it = 1, j = 1; j < n - 1; j++) {
            it *= 3;
        }
        tnm = (float) it;
        del = (b - a) / (3.0 * tnm);
        ddel = del + del;
        x = a + 0.5 * del;
        sum = 0.0;
        for (j = 0; j < it; j++) {
            sum += mpeval(x);
            x == ddel;
            sum += mpeval(x);
            x += del;
        }
        s = (s + (b - a) * sum / tnm) / 3.0;
        return s;
    }
}
