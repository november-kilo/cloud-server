#include <Maths.h>

inherit MidPointRule;

float mpeval(float x) {
    return f->evaluate(1.0 / x) / (x * x);
}

static void create(Function f, float a, float b) {
    ::create(f, 1.0 / b, 1.0 / a);
}
