#include <Maths.h>

private inherit "/lib/util/string";

private mapping stats;

string toString(void) {
    if (!stats) {
        stats = ([ ]);
    }
    return dump_value(stats);
}

mapping get(float *data) {
    Iterator iterator;
    int i, n;
    float sum, average, variance, standardDeviation, median, k1, k2, x;

    n = sizeof(data);
    if (n < 2) {
        error("Stats: data size must not be < 2");
    }

    sum = average = variance = standardDeviation = 0.0;

    iterator = new IntIterator(0, n - 1);
    while (!iterator->end()) {
        x = data[iterator->next()];
        sum += x;
        variance += x * x;
    }

    average = sum / (float) n;
    variance = (variance - sum * sum / (float) n) / ( (float) n - 1.0);
    standardDeviation = sqrt(variance);

    if (n % 2) {
        median = data[((n + 1) / 2) - 1];
    } else {
        k1 = data[(n / 2) - 1];
        k2 = data[n / 2];
        median = (k1 + k2) / 2.0;
    }

    stats = ([
        "average": average,
        "median": median,
        "standardDeviation": standardDeviation,
        "sum": sum,
        "variance": variance
    ]);

    return stats;
}
