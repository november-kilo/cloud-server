#include <Maths.h>

inherit Function;

float evaluate(mixed x) {
    return exp(new Number(x)->toFloat());
}
