#include <Math.h>

#define K 5

inherit Integrator;

float integrate(Function f, float lowerLimit, float upperLimit, varargs float tolerance) {
    int j;
    float *s, *h;
    float returnValue;
    PolynomialInterpolator interpolator;
    TrapezoidQuadrature trapezoidQuadrature;

    s = allocate_float(MAX_INTEGRATION_ITERATIONS);
    h = allocate_float(MAX_INTEGRATION_ITERATIONS + 1);
    interpolator = new PolynomialInterpolator(h, s, K);
    h[0] = 1.0;
    trapezoidQuadrature = new TrapezoidQuadrature(f, lowerLimit, upperLimit);

    if (!tolerance) {
        tolerance = TOLERANCE;
    }

    for (j = 1; j <= MAX_INTEGRATION_ITERATIONS; j++) {
        s[j - 1] = trapezoidQuadrature->next();

        if (j >= K) {
            returnValue = interpolator->interpolate(j - K, 0.0);

            if (fabs(interpolator->getDy()) <= tolerance * fabs(returnValue)) {
                return returnValue;
            }
        }

        h[j] = 0.25 * h[j - 1];
    }

    error("Romberg: too many steps");
}
