#include <Math.h>

#define N 5

inherit Integrator;

float *lroots;
float *weight;
float **lcoef;

private void gaussLegendreCoefficients(void) {
    Iterator iterator, innerIterator;
    int n, i;

    lcoef[0][0] = lcoef[1][1] = 1.0;
    iterator = new IntIterator(2, N);
    while (!iterator->end()) {
        n = iterator->next();

        lcoef[n][0] = -((float) n - 1.0) * lcoef[n - 2][0] / (float) n;

        innerIterator = new IntIterator(1, n);
        while (!innerIterator->end()) {
            i = innerIterator->next();

            lcoef[n][i] = ((2.0 * (float) n - 1.0) * lcoef[n - 1][i - 1] - ((float) n - 1.0) * lcoef[n - 2][i]) / (float) n;
        }
    }
}

private float gaussLegendreEval(int n, float x) {
    Iterator iterator;
    float s;

    s = lcoef[n][n];
    iterator = new IntIterator(n, 1);
    while (!iterator->end()) {
        s = s * x + lcoef[n][iterator->next() - 1];
    }

    return s;
}

private float gaussLegendreDiff(int n, float x) {
    return (float) n * (x * gaussLegendreEval(n, x) - gaussLegendreEval(n - 1, x)) / (x * x - 1.0);
}

private void gaussLegendreRoots(void) {
    Iterator iterator;
    int i;
    float x, x1;

    iterator = new IntIterator(1, N);
    while (!iterator->end()) {
        i = iterator->next();

        x = cos(pi() * ((float)i - .25) / ((float) N + .5));

        do {
            x1 = x;
            x -= gaussLegendreEval(N, x) / gaussLegendreDiff(N, x);
        } while (fabs(x - x1) > TOLERANCE);

        lroots[i - 1] = x;

        x1 = gaussLegendreDiff(N, x);
        weight[i - 1] = 2.0 / ((1.0 - x * x) * x1 * x1);
    }
}

float integrate(Function f, float lowerLimit, float upperLimit, varargs float tolerance) {
    float x1, x2, sum;
    Iterator iterator;

    x1 = (upperLimit - lowerLimit) / 2.0;
    x2 = (upperLimit + lowerLimit) / 2.0;
    sum = 0.0;

    iterator = new IntIterator(0, N - 1);
    while (!iterator->end()) {
        sum += weight[iterator->next()] * f->evaluate(x1 * lroots[iterator->current()] + x2);
    }

    return x1 * sum;
}

static void create(void) {
    Iterator iterator;

    lroots = allocate_float(N);
    weight = allocate_float(N);
    lcoef = allocate(N + 1);

    iterator = new IntIterator(0, N);
    while (!iterator->end()) {
        lcoef[iterator->next()] = allocate_float(N + 1);
    }

    gaussLegendreCoefficients();
    gaussLegendreRoots();
}
