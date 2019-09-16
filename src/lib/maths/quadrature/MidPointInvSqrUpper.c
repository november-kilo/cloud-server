#include <Maths.h>

inherit MidPointRule;

static float borig;

float mpeval(float x) {
    return 2.0 * x * f->evaluate(borig + x * x);
}

static void create(Function f, float a, float b) {
    borig = b;
    ::create(f, 0.0, sqrt(b - a));
}
