#include <Maths.h>

inherit Integrator;

float integrate(Function f, float lowerLimit, float upperLimit, varargs float tolerance) {
    if (!tolerance) {
        tolerance = TOLERANCE;
    }

    return new SimpsonQuad()->evaluate(f, lowerLimit, upperLimit, tolerance);
}
