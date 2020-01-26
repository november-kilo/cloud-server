#include <Math.h>

static int n;
static float a;
static float b;
static Function f;
static float x;

static void create(Function f, float a, float b) {
    ::f = f;
    ::a = a;
    ::b = b;
    n = 0;
    x = 0.0;
}

float current(void) {
    return x;
}

float next(void) {
    catch { x += 1.0; } : { x = 0.0; };
    return x;
}
