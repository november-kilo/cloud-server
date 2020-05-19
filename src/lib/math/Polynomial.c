private float *coeffs;

static void create(varargs float *coeffs) {
    if (!coeffs || !sizeof(coeffs)) {
        error("Polynomial: parameters cannot be empty");
    }

    ::coeffs = coeffs;
}

float *coefficients(void) {
    return coeffs[..];
}

void foo(object p) {
    coefficients()[0] + (-1.0 * p->coefficients()[0]);
}
