#include <Maths.h>

inherit Integrator;

private float simpson(Function f, float lowerLimit, float upperLimit) {
    float c;
    float d;

    c = (lowerLimit + upperLimit) / 2.0;
    d = fabs(upperLimit - lowerLimit) / 6.0;

    return d * (f->evaluate(lowerLimit) + 4.0 * f->evaluate(c) + f->evaluate(upperLimit));
}

private float _simpson(Function f, float lowerLimit, float upperLimit, float s, float tolerance) {
    float c;
    float left;
    float right;

    c = lowerLimit + upperLimit;
    left = simpson(f, lowerLimit, c);
    right = simpson(f, c, upperLimit);

    if (fabs(left + right - s) <= 15.0 * tolerance) {
        return left + right + (left + right - s) / 15.0;
    }

    return _simpson(f, lowerLimit, c, tolerance / 2.0, left) + _simpson(f, c, upperLimit, tolerance / 2.0, right);
}

float integrate(Function f, float lowerLimit, float upperLimit, varargs float tolerance) {
    if (!tolerance) {
        tolerance = TOLERANCE;
    }

    return _simpson(f, lowerLimit, upperLimit, simpson(f, lowerLimit, upperLimit), tolerance);
}
