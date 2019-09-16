#include <Maths.h>

inherit MidPointRule;

static float aorig;

float mpeval(float x) {
    return 2.0 * x * f->evaluate(aorig + x * x);
}

static void create(Function f, float a, float b) {
    aorig = a;
    ::create(f, 0.0, sqrt(b - a));
}
