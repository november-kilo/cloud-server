#include <Maths.h>

inherit Function;

float evaluate(Function f, float a, float b, varargs float tolerance) {
    int j, jmax;
    float s, st, ost, os;
    TrapezoidQuadrature trapdz;

    if (!tolerance) {
        tolerance = TOLERANCE;
    }

    ost = 0.0;
    os = 0.0;
    jmax = MAX_INTEGRATION_ITERATIONS;
    trapdz = new TrapezoidQuadrature(f, a, b);

    for (j = 0; j < jmax; j++) {
        st = trapdz->next();
        s = (4.0 * st - ost) / 3.0;
        if (j > 5) {
            if (fabs(s - os) < tolerance * fabs(os) || (s == 0.0 && os == 0.0)) {
                return s;
            }
            os = s;
            ost = st;
        }
    }

    error("Simpson: too many steps");
}
