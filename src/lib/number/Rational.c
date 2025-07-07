#include <Number.h>

private int numerator;
private int denominator;
private float original_float;

private void fix_sign(void) {
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
}

private int gcd(int x, int y) {
    int a, b;

    a = (x > 0) ? x : -x;
    b = (y > 0) ? y : -y;

    while(a != b)  {
        if(a > b)
            a -= b;
        else
            b -= a;
    }

    return a;
}

private void to_canonical(void) {
    int g;
    g = gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;
}

int numerator(void) {
    return numerator;
}

int denominator(void) {
    return denominator;
}

float original_float(void) {
    return original_float;
}

void from_rational(int numerator, int denominator) {
    ::numerator = numerator;
    ::denominator = denominator;
    if (!denominator) {
        error("Rational: the denominator must be different from zero");
    }
    fix_sign();
    if (numerator != 0) {
        to_canonical();
    }
}

void float_from(float f) {
    float f_copy, f_copy_floor, n0, n1, n2, d0, d1, d2;
    float one_over, one_over_floor;
    int n;

    original_float = f;

    if (f == 1.) {
        numerator = 1;
        denominator = 1;
        return;
    }

    if (f == 0.) {
        numerator = 0;
        denominator = 1;
        return;
    }

    f_copy = f;
    f_copy_floor = floor(f_copy);
    n0 = 1.0;
    n1 = f_copy_floor;
    n2 = 0.0;
    d0 = 0.0;
    d1 = 1.0;
    d2 = 1.0;

    if (fabs(f_copy_floor - f) <= TOLERANCE) {
        numerator = (int) f_copy_floor;
        denominator = 1;
        return;
    }

    for (n = 0; n < MAX_ITERATIONS; n++) {
        one_over = 1.0 / (f_copy - f_copy_floor);
        one_over_floor = floor(one_over);
        n2 = one_over_floor * n1 + n0;
        d2 = one_over_floor * d1 + d0;

        if (fabs(n2 / d2 -f) <= TOLERANCE) {
            break;
        }

        n0 = n1;
        n1 = n2;
        d0 = d1;
        d1 = d2;
        f_copy_floor = one_over_floor;
        f_copy = one_over;
    }

    numerator = (int) n2;
    denominator = (int) d2;
}

void reciprocal(void) {
    int tmp;

    if (numerator == 0) {
        error("Rational: the operation would result in a denominator of zero");
    }

    tmp = denominator;
    denominator = numerator;
    numerator = tmp;
    fix_sign();
    to_canonical();
}

private void instance_of_bad_type(mixed x) {
    error("Rational: cannot operate with value of type " + typeof(x));
}

private Rational mixed_to_rational(mixed x) {
    float f;

    switch (typeof(x)) {
        case T_FLOAT:
            return new Rational(x);
        case T_INT:
            return new Rational(x, 1);
        case T_STRING:
            if (catch(f = (float) x)) {
                instance_of_bad_type(x);
            }
            return new Rational(f);
        case T_OBJECT:
            if (x <- Number) {
                return new Rational(x->to_float());
            }
    }

    instance_of_bad_type(x);
}

private int* make_common(Rational r) {
    int n1, n2, d1;

    n1 = numerator;
    d1 = denominator;
    n2 = r->numerator();

    n1 *= r->denominator();
    d1 *= r->denominator();
    n2 *= denominator;

    return ({ n1, n2, d1 });
}

static Rational operator + (mixed r) {
    int n1, n2, d1;
    
    ({ n1, n2, d1 }) = make_common(mixed_to_rational(r));
    
    return new Rational(n1 + n2, d1);
}

static Rational operator - (mixed r) {
    int n1, n2, d1;
    
    ({ n1, n2, d1 }) = make_common(mixed_to_rational(r));
    
    return new Rational(n1 - n2, d1);
}

static Rational operator ^ (mixed p) {
    int i;
    
    switch (typeof(p)) {
        case T_OBJECT:
            if (p <- Number) {
                i = p->to_int();
                break;
            }
            error("Rational: invalid power (object)");
        case T_INT:
        case T_FLOAT:
            i = (int) p;
            break;
        case T_STRING:
            if (catch(i = (int)(float) p)) {
                error("Rational: invalid power (string)");
            }
            break;
        default:
            error("Rational: invalid power (default)");
    }
    
    if (i == 0) {
        return new Rational(1, 1);
    }
    
    return new Rational((int) pow((float) numerator, (float) i), (int) pow((float) denominator, (float) i));
}

static Rational operator * (mixed r) {
    r = mixed_to_rational(r);

    return new Rational(numerator * r->numerator(), denominator * r->denominator());
}

static Rational operator / (mixed r) {
    Rational rr;
    
    r = mixed_to_rational(r);
    rr = new Rational(r->numerator(), r->denominator());
    rr->reciprocal();
    
    return new Rational(numerator, denominator) * rr;
}

static int operator < (mixed r) {
    int n1, n2;

    ({ n1, n2 }) = make_common(mixed_to_rational(r));

    return n1 < n2;
}

static int operator <= (mixed r) {
    int n1, n2;
    
    ({ n1, n2 }) = make_common(mixed_to_rational(r));
    
    return n1 <= n2;
}

static int operator > (mixed r) {
    int n1, n2;
    
    ({ n1, n2 }) = make_common(mixed_to_rational(r));
    
    return n1 > n2;
}

static int operator >= (mixed r) {
    int n1, n2;
    
    ({ n1, n2 }) = make_common(mixed_to_rational(r));
    
    return n1 >= n2;
}

int equals(mixed r) {
    int n1, n2;

    ({ n1, n2 }) = make_common(mixed_to_rational(r));

    return n1 == n2;
}

string to_string(void) {
    return "" + numerator + "/" + denominator;
}

float to_float(void) {
    return (float) numerator / (float) denominator;
}

int to_int(void) {
    return numerator / denominator;
}

static void create(Number args...) {
    if (sizeof(args) == 1) {
        if (typeof(args[0]) == T_OBJECT) {
            if (args[0] <- Rational) {
                from_rational(args[0]->numerator(), args[0]->denominator());
                return;
            }

            if (modf(args[0]->to_float())[0] == 0.0) {
                from_rational(args[0]->to_int(), 1);
                return;
            }

            float_from(args[0]->to_float());
            return;
        }
    }

    if (sizeof(args) == 2) {
        from_rational(args[0]->to_int(), args[1]->to_int());
        return;
    }

    error("Rational: invalid create parameters");
}
