#include <Math.h>

inherit Quadrature;

static float s;

static void create(Function f, float a, float b) {
    ::s = 0.0;
    ::create(f, a, b);
}

float next(void) {
    float x, tnm, sum, del;
    int it, j;

    n++;

    if (n == 1) {
        return (s = 0.5 * (b - a) * (f->evaluate(a) + f->evaluate(b)));
    } else {
        for (it = 1, j = 1; j < n - 1; j++) {
            it <<= 1;
        }
        tnm = (float) it;
        del = (b - a) / tnm;
        x = a + 0.5 * del;
        for (sum = 0.0, j = 0; j < it; j++, x += del) {
            sum += f->evaluate(x);
        }
        s = 0.5 * (s + (b - a) * sum / tnm);
        return s;
    }
}
