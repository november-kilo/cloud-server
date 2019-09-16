#include <Maths.h>

inherit MidPointRule;

float mpeval(float x) {
    return f->evaluate(-log(x)) / x;
}

static void create(Function f, float a, float b) {
    ::create(f, 0.0, exp(-a));
}
