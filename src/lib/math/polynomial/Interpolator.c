static float *x;
static float *y;
static int m;
static int n;
static float dy;

static void create(float *x, float *y, int m) {
    ::x = x;
    ::y = y;
    ::m = m;
    ::n = sizeof(x);
    dy = 0.0;
}

float getDy(void) {
    return dy;
}

float interpolate(int jl, float a) {
    int i, k, ns;
    float returnValue, den, dif, dift, ho, hp, w;
    float *xa, *ya, *c, *d;

    ns = 0;
    xa = x[jl..];
    ya = y[jl..];
    c = allocate_float(m);
    d = allocate_float(m);

    dif = fabs(a - xa[0]);

    for (i = 0; i < m; i++) {
        if ((dift = fabs(a - xa[i])) < dif) {
            ns = i;
            dif = dift;
        }

        c[i] = ya[i];
        d[i] = ya[i];
    }

    returnValue = ya[ns--];

    for (k = 1; k < m; k++) {
        for (i = 0; i < m - k; i++) {
            ho = xa[i] - a;
            hp = xa[i + k] - a;
            w = c[i + 1] - d[i];

            if ((den = ho - hp) == 0.0) {
                error("PolynomialInterpolator: interpolation error");
            }

            den = w / den;
            d[i] = hp * den;
            c[i] = ho * den;
        }

        returnValue += (dy = (2 * (ns + 1) < (m - k) ? c[ns + 1] : d[ns--]));
    }

    return returnValue;
}