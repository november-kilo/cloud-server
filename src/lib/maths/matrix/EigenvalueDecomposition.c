#include <Maths.h>

private int n;
private int issymmetric;
private float *d;
private float *e;
private float **V;
private float **H;
private float *ort;
private float cdivr;
private float cdivi;

private void cdiv(float xr, float xi, float yr, float yi) {
    Complex z;

    z = new Complex(xr, xi) / new Complex(yr, yi);

    cdivr = z->re();
    cdivi = z->im();
}

/* Symmetric Householder reduction to tridiagonal form. */

private void tred2(void) {
    /*
     * This is derived from the Algol procedures tred2 by
     * Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
     * Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
     * Fortran subroutine in EISPACK.
     */
    int i, j, k;
    float f, g, h, hh, scale;

    for (j = 0; j < n; j++) {
        d[j] = V[n-1][j];
    }

    /* Householder reduction to tridiagonal form. */

    for (i = n-1; i > 0; i--) {

        /* Scale to avoid under/overflow. */

        scale = 0.;
        h = 0.;
        for (k = 0; k < i; k++) {
            scale = scale + fabs(d[k]);
        }
        if (scale == 0.) {
            e[i] = d[i-1];
            for (j = 0; j < i; j++) {
                d[j] = V[i-1][j];
                V[i][j] = 0.;
                V[j][i] = 0.;
            }
        } else {

            /* Generate Householder vector. */

            for (k = 0; k < i; k++) {
                d[k] /= scale;
                h += d[k] * d[k];
            }
            f = d[i-1];
            g = sqrt(h);
            if (f > 0.) {
                g = -g;
            }
            e[i] = scale * g;
            h = h - f * g;
            d[i-1] = f - g;
            for (j = 0; j < i; j++) {
                e[j] = 0.;
            }

            /* Apply similarity transformation to remaining columns. */

            for (j = 0; j < i; j++) {
                f = d[j];
                V[j][i] = f;
                g = e[j] + V[j][j] * f;
                for (k = j + 1; k <= i - 1; k++) {
                    g += V[k][j] * d[k];
                    e[k] += V[k][j] * f;
                }
                e[j] = g;
            }
            f = 0.;
            for (j = 0; j < i; j++) {
                e[j] /= h;
                f += e[j] * d[j];
            }
            hh = f / (h + h);
            for (j = 0; j < i; j++) {
                e[j] -= hh * d[j];
            }
            for (j = 0; j < i; j++) {
                f = d[j];
                g = e[j];
                for (k = j; k <= i-1; k++) {
                    V[k][j] -= (f * e[k] + g * d[k]);
                }
                d[j] = V[i - 1][j];
                V[i][j] = 0.;
            }
        }
        d[i] = h;
    }

    /* Accumulate transformations. */

    for (i = 0; i < n - 1; i++) {
        V[n - 1][i] = V[i][i];
        V[i][i] = 1.;
        h = d[i + 1];
        if (h != 0.) {
            for (k = 0; k <= i; k++) {
                d[k] = V[k][i + 1] / h;
            }
            for (j = 0; j <= i; j++) {
                g = 0.;
                for (k = 0; k <= i; k++) {
                    g += V[k][i + 1] * V[k][j];
                }
                for (k = 0; k <= i; k++) {
                    V[k][j] -= g * d[k];
                }
            }
        }
        for (k = 0; k <= i; k++) {
            V[k][i + 1] = 0.;
        }
    }
    for (j = 0; j < n; j++) {
        d[j] = V[n - 1][j];
        V[n - 1][j] = 0.;
    }
    V[n - 1][n - 1] = 1.;
    e[0] = 0.;
}

/* Symmetric tridiagonal QL algorithm. */

private void tql2(void) {
    /*
     * This is derived from the Algol procedures tql2, by
     * Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
     * Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
     * Fortran subroutine in EISPACK.
     */
    int i, j, k, l, iter, m;
    float c, c2, c3, dl1, el1, f, g, h, p, r, s, s2, tst1, eps;

    for (i = 1; i < n; i++) {
        e[i - 1] = e[i];
    }
    e[n - 1] = 0.;

    f = 0.;
    tst1 = 0.;
    eps = pow(2., -52.);
    for (l = 0; l < n; l++) {

        /* Find small subdiagonal element */

        tst1 = max(tst1, fabs(d[l]) + fabs(e[l]));
        m = l;
        while (m < n) {
            if (fabs(e[m]) <= eps * tst1) {
                break;
            }
            m++;
        }

        /*
         * If m == l, d[l] is an eigenvalue,
         * otherwise, iterate.
         */
        if (m > l) {
            iter = 0;
            do {
                iter = iter + 1;  /* (Could check iteration count here.) */

                /* Compute implicit shift */

                g = d[l];
                p = (d[l + 1] - g) / (2. * e[l]);
                r = hypot(p, 1.);
                if (p < 0.) {
                    r = -r;
                }
                d[l] = e[l] / (p + r);
                d[l + 1] = e[l] * (p + r);
                dl1 = d[l + 1];
                h = g - d[l];
                for (i = l + 2; i < n; i++) {
                    d[i] -= h;
                }
                f = f + h;

                /* Implicit QL transformation. */

                p = d[m];
                c = 1.;
                c2 = c;
                c3 = c;
                el1 = e[l + 1];
                s = 0.;
                s2 = 0.;
                for (i = m - 1; i >= l; i--) {
                    c3 = c2;
                    c2 = c;
                    s2 = s;
                    g = c * e[i];
                    h = c * p;
                    r = hypot(p, e[i]);
                    e[i + 1] = s * r;
                    s = e[i] / r;
                    c = p / r;
                    p = c * d[i] - s * g;
                    d[i + 1] = h + s * (c * g + s * d[i]);

                    /* Accumulate transformation. */

                    for (k = 0; k < n; k++) {
                        h = V[k][i + 1];
                        V[k][i + 1] = s * V[k][i] + c * h;
                        V[k][i] = c * V[k][i] - s * h;
                    }
                }
                p = -s * s2 * c3 * el1 * e[l] / dl1;
                e[l] = s * p;
                d[l] = c * p;

                /* Check for convergence. */

            } while (fabs(e[l]) > eps * tst1);
        }
        d[l] = d[l] + f;
        e[l] = 0.;
    }

    /* Sort eigenvalues and corresponding vectors. */

    for (i = 0; i < n - 1; i++) {
        k = i;
        p = d[i];
        for (j = i + 1; j < n; j++) {
            if (d[j] < p) {
                k = j;
                p = d[j];
            }
        }
        if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j++) {
                p = V[j][i];
                V[j][i] = V[j][k];
                V[j][k] = p;
            }
        }
    }
}

/* Nonsymmetric reduction to Hessenberg form. */

private void orthes(void) {
    /*
     * This is derived from the Algol procedures orthes and ortran,
     * by Martin and Wilkinson, Handbook for Auto. Comp.,
     * Vol.ii-Linear Algebra, and the corresponding
     *Fortran subroutines in EISPACK.
     */
    int low, high, i, j, m;
    float f, g, h, scale;

    low = 0;
    high = n-1;

    for (m = low + 1; m <= high - 1; m++) {

        /* Scale column. */

        scale = 0.;
        for (i = m; i <= high; i++) {
            scale = scale + fabs(H[i][m - 1]);
        }
        if (scale != 0.) {

            /* Compute Householder transformation. */

            h = 0.;
            for (i = high; i >= m; i--) {
                ort[i] = H[i][m - 1] / scale;
                h += ort[i] * ort[i];
            }
            g = sqrt(h);
            if (ort[m] > 0.) {
                g = -g;
            }
            h = h - ort[m] * g;
            ort[m] = ort[m] - g;

            /*
             * Apply Householder similarity transformation
             * H = (I-u*u'/h)*H*(I-u*u')/h)
             */

            for (j = m; j < n; j++) {
                f = 0.;
                for (i = high; i >= m; i--) {
                    f += ort[i] * H[i][j];
                }
                f = f / h;
                for (i = m; i <= high; i++) {
                    H[i][j] -= f * ort[i];
                }
            }

            for (i = 0; i <= high; i++) {
                f = 0.;
                for (j = high; j >= m; j--) {
                    f += ort[j] * H[i][j];
                }
                f = f / h;
                for (j = m; j <= high; j++) {
                    H[i][j] -= f * ort[j];
                }
            }
            ort[m] = scale * ort[m];
            H[m][m - 1] = scale * g;
        }
    }

    /* Accumulate transformations (Algol's ortran). */

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            V[i][j] = (i == j ? 1. : 0.);
        }
    }

    for (m = high - 1; m >= low + 1; m--) {
        if (H[m][m - 1] != 0.) {
            for (i = m + 1; i <= high; i++) {
                ort[i] = H[i][m - 1];
            }
            for (j = m; j <= high; j++) {
                g = 0.;
                for (i = m; i <= high; i++) {
                    g += ort[i] * V[i][j];
                }
                /* Double division avoids possible underflow */
                g = (g / ort[m]) / H[m][m - 1];
                for (i = m; i <= high; i++) {
                    V[i][j] += g * ort[i];
                }
            }
        }
    }
}

/* Nonsymmetric reduction from Hessenberg to real Schur form. */

private void hqr2(void) {
    /*
     * This is derived from the Algol procedure hqr2,
     * by Martin and Wilkinson, Handbook for Auto. Comp.,
     * Vol.ii-Linear Algebra, and the corresponding
     * Fortran subroutine in EISPACK.
     */

    int i, j, k, iter, l, m, nn, n, low, high, notlast;
    float eps, exshift, norm, p, q, r, s, z, t, w, x, y;
    float ra, sa, vr, vi;

    /* Initialize */

    nn = ::n;
    n = nn - 1;
    low = 0;
    high = nn - 1;
    eps = pow(2., -52.);
    exshift = 0.;
    p = 0.;
    q = 0.;
    r = 0.;
    s = 0.;
    z = 0.;

    /* Store roots isolated by balanc and compute matrix norm */

    norm = 0.;
    for (i = 0; i < nn; i++) {
        if (i < low | i > high) {
            d[i] = H[i][i];
            e[i] = 0.;
        }
        for (j = max(i - 1, 0); j < nn; j++) {
            norm = norm + fabs(H[i][j]);
        }
    }

    /* Outer loop over eigenvalue index */

    iter = 0;
    while (n >= low) {

        /* Look for single small sub-diagonal element */

        l = n;
        while (l > low) {
            s = fabs(H[l - 1][l - 1]) + fabs(H[l][l]);
            if (s == 0.) {
                s = norm;
            }
            if (fabs(H[l][l-1]) < eps * s) {
                break;
            }
            l--;
        }

        /* Check for convergence */
        /* One root found */

        if (l == n) {
            H[n][n] = H[n][n] + exshift;
            d[n] = H[n][n];
            e[n] = 0.;
            n--;
            iter = 0;

            /* Two roots found */

        } else if (l == n - 1) {
            w = H[n][n - 1] * H[n-1][n];
            p = (H[n - 1][n-1] - H[n][n]) / 2.;
            q = p * p + w;
            z = sqrt(fabs(q));
            H[n][n] = H[n][n] + exshift;
            H[n - 1][n - 1] = H[n - 1][n - 1] + exshift;
            x = H[n][n];

            /* Real pair */

            if (q >= 0.) {
                if (p >= 0.) {
                    z = p + z;
                } else {
                    z = p - z;
                }
                d[n - 1] = x + z;
                d[n] = d[n - 1];
                if (z != 0.) {
                    d[n] = x - w / z;
                }
                e[n - 1] = 0.;
                e[n] = 0.;
                x = H[n][n - 1];
                s = fabs(x) + fabs(z);
                p = x / s;
                q = z / s;
                r = sqrt(p * p+q * q);
                p = p / r;
                q = q / r;

                /* Row modification */

                for (j = n - 1; j < nn; j++) {
                    z = H[n - 1][j];
                    H[n - 1][j] = q * z + p * H[n][j];
                    H[n][j] = q * H[n][j] - p * z;
                }

                /* Column modification */

                for (i = 0; i <= n; i++) {
                    z = H[i][n - 1];
                    H[i][n - 1] = q * z + p * H[i][n];
                    H[i][n] = q * H[i][n] - p * z;
                }

                /* Accumulate transformations */

                for (i = low; i <= high; i++) {
                    z = V[i][n - 1];
                    V[i][n - 1] = q * z + p * V[i][n];
                    V[i][n] = q * V[i][n] - p * z;
                }

                /* Complex pair */

            } else {
                d[n - 1] = x + p;
                d[n] = x + p;
                e[n - 1] = z;
                e[n] = -z;
            }
            n = n - 2;
            iter = 0;

            /* No convergence yet */

        } else {

            /* Form shift */

            x = H[n][n];
            y = 0.;
            w = 0.;
            if (l < n) {
                y = H[n - 1][n - 1];
                w = H[n][n - 1] * H[n - 1][n];
            }

            /* Wilkinson's original ad hoc shift */

            if (iter == 10) {
                exshift += x;
                for (i = low; i <= n; i++) {
                    H[i][i] -= x;
                }
                s = fabs(H[n][n - 1]) + fabs(H[n - 1][n-2]);
                x = y = 0.75 * s;
                w = -0.4375 * s * s;
            }

            /* MATLAB's new ad hoc shift */

            if (iter == 30) {
                s = (y - x) / 2.;
                s = s * s + w;
                if (s > 0.) {
                    s = sqrt(s);
                    if (y < x) {
                        s = -s;
                    }
                    s = x - w / ((y - x) / 2. + s);
                    for (i = low; i <= n; i++) {
                        H[i][i] -= s;
                    }
                    exshift += s;
                    x = y = w = 0.964;
                }
            }

            iter = iter + 1;   /* (Could check iteration count here.) */

            /* Look for two consecutive small sub-diagonal elements */

            m = n - 2;
            while (m >= l) {
                z = H[m][m];
                r = x - z;
                s = y - z;
                p = (r * s - w) / H[m + 1][m] + H[m][m+1];
                q = H[m + 1][m + 1] - z - r - s;
                r = H[m + 2][m + 1];
                s = fabs(p) + fabs(q) + fabs(r);
                p = p / s;
                q = q / s;
                r = r / s;
                if (m == l) {
                    break;
                }
                if (fabs(H[m][m - 1]) * (fabs(q) + fabs(r)) <
                    eps * (fabs(p) * (fabs(H[m - 1][m - 1]) + fabs(z) +
                                      fabs(H[m + 1][m + 1])))) {
                    break;
                }
                m--;
            }

            for (i = m + 2; i <= n; i++) {
                H[i][i - 2] = 0.;
                if (i > m + 2) {
                    H[i][i - 3] = 0.;
                }
            }

            /* Double QR step involving rows l:n and columns m:n */


            for (k = m; k <= n - 1; k++) {
                notlast = (k != n - 1);
                if (k != m) {
                    p = H[k][k - 1];
                    q = H[k + 1][k - 1];
                    r = (notlast ? H[k + 2][k - 1] : 0.);
                    x = fabs(p) + fabs(q) + fabs(r);
                    if (x == 0.) {
                        continue;
                    }
                    p = p / x;
                    q = q / x;
                    r = r / x;
                }

                s = sqrt(p * p + q * q + r * r);
                if (p < 0.) {
                    s = -s;
                }
                if (s != 0.) {
                    if (k != m) {
                        H[k][k - 1] = -s * x;
                    } else if (l != m) {
                        H[k][k - 1] = -H[k][k - 1];
                    }
                    p = p + s;
                    x = p / s;
                    y = q / s;
                    z = r / s;
                    q = q / p;
                    r = r / p;

                    /* Row modification */

                    for (j = k; j < nn; j++) {
                        p = H[k][j] + q * H[k + 1][j];
                        if (notlast) {
                            p = p + r * H[k + 2][j];
                            H[k + 2][j] = H[k + 2][j] - p * z;
                        }
                        H[k][j] = H[k][j] - p * x;
                        H[k + 1][j] = H[k + 1][j] - p * y;
                    }

                    /* Column modification */

                    for (i = 0; i <= min(n, k + 3); i++) {
                        p = x * H[i][k] + y * H[i][k + 1];
                        if (notlast) {
                            p = p + z * H[i][k + 2];
                            H[i][k + 2] = H[i][k + 2] - p * r;
                        }
                        H[i][k] = H[i][k] - p;
                        H[i][k + 1] = H[i][k + 1] - p * q;
                    }

                    /* Accumulate transformations */

                    for (i = low; i <= high; i++) {
                        p = x * V[i][k] + y * V[i][k + 1];
                        if (notlast) {
                            p = p + z * V[i][k + 2];
                            V[i][k + 2] = V[i][k + 2] - p * r;
                        }
                        V[i][k] = V[i][k] - p;
                        V[i][k + 1] = V[i][k + 1] - p * q;
                    }
                }  /* (s != 0) */
            }  /* k loop */
        }  /* check convergence */
    }  /* while (n >= low) */

    /* Backsubstitute to find vectors of upper triangular form */

    if (norm == 0.) {
        return;
    }

    for (n = nn - 1; n >= 0; n--) {
        p = d[n];
        q = e[n];

        /* Real vector */

        if (q == 0.) {
            l = n;
            H[n][n] = 1.;
            for (i = n - 1; i >= 0; i--) {
                w = H[i][i] - p;
                r = 0.;
                for (j = l; j <= n; j++) {
                    r = r + H[i][j] * H[j][n];
                }
                if (e[i] < 0.) {
                    z = w;
                    s = r;
                } else {
                    l = i;
                    if (e[i] == 0.) {
                        if (w != 0.) {
                            H[i][n] = -r / w;
                        } else {
                            H[i][n] = -r / (eps * norm);
                        }

                        /* Solve real equations */

                    } else {
                        x = H[i][i + 1];
                        y = H[i + 1][i];
                        q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                        t = (x * s - z * r) / q;
                        H[i][n] = t;
                        if (fabs(x) > fabs(z)) {
                            H[i + 1][n] = (-r - w * t) / x;
                        } else {
                            H[i + 1][n] = (-s - y * t) / z;
                        }
                    }

                    /* Overflow control */

                    t = fabs(H[i][n]);
                    if ((eps * t) * t > 1.) {
                        for (j = i; j <= n; j++) {
                            H[j][n] = H[j][n] / t;
                        }
                    }
                }
            }

            /* Complex vector */

        } else if (q < 0.) {
            l = n - 1;

            /* Last vector component imaginary so matrix is triangular */

            if (fabs(H[n][n - 1]) > fabs(H[n - 1][n])) {
                H[n - 1][n - 1] = q / H[n][n - 1];
                H[n - 1][n] = -(H[n][n] - p) / H[n][n - 1];
            } else {
                cdiv(0., -H[n - 1][n], H[n - 1][n - 1]-p, q);
                H[n - 1][n - 1] = cdivr;
                H[n - 1][n] = cdivi;
            }
            H[n][n - 1] = 0.;
            H[n][n] = 1.;
            for (i = n - 2; i >= 0; i--) {
                ra = 0.;
                sa = 0.;
                for (j = l; j <= n; j++) {
                    ra = ra + H[i][j] * H[j][n - 1];
                    sa = sa + H[i][j] * H[j][n];
                }
                w = H[i][i] - p;

                if (e[i] < 0.) {
                    z = w;
                    r = ra;
                    s = sa;
                } else {
                    l = i;
                    if (e[i] == 0.) {
                        cdiv(-ra, -sa, w, q);
                        H[i][n - 1] = cdivr;
                        H[i][n] = cdivi;
                    } else {

                        /* Solve complex equations */

                        x = H[i][i + 1];
                        y = H[i + 1][i];
                        vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                        vi = (d[i] - p) * 2. * q;
                        if (vr == 0. & vi == 0.) {
                            vr = eps * norm * (fabs(w) + fabs(q) +
                                               fabs(x) + fabs(y) + fabs(z));
                        }
                        cdiv(x * r - z * ra + q * sa, x * s - z * sa - q * ra, vr, vi);
                        H[i][n - 1] = cdivr;
                        H[i][n] = cdivi;
                        if (fabs(x) > (fabs(z) + fabs(q))) {
                            H[i + 1][n - 1] = (-ra - w * H[i][n - 1] + q * H[i][n]) / x;
                            H[i + 1][n] = (-sa - w * H[i][n] - q * H[i][n - 1]) / x;
                        } else {
                            cdiv(-r - y * H[i][n - 1], -s - y * H[i][n], z, q);
                            H[i + 1][n - 1] = cdivr;
                            H[i + 1][n] = cdivi;
                        }
                    }

                    /* Overflow control */

                    t = max(fabs(H[i][n - 1]), fabs(H[i][n]));
                    if ((eps * t) * t > 1.) {
                        for (j = i; j <= n; j++) {
                            H[j][n - 1] = H[j][n - 1] / t;
                            H[j][n] = H[j][n] / t;
                        }
                    }
                }
            }
        }
    }

    /* Vectors of isolated roots */

    for (i = 0; i < nn; i++) {
        if (i < low | i > high) {
            for (j = i; j < nn; j++) {
                V[i][j] = H[i][j];
            }
        }
    }

    /* Back transformation to get eigenvectors of original matrix */

    for (j = nn - 1; j >= low; j--) {
        for (i = low; i <= high; i++) {
            z = 0.;
            for (k = low; k <= min(j, high); k++) {
                z = z + V[i][k] * H[k][j];
            }
            V[i][j] = z;
        }
    }
}

Matrix getV(void) {
    Matrix matrix;

    matrix = new Matrix(n, n);
    matrix->setMatrix(V);

    return matrix;
}

float *getRealEigenvalues(void) {
    return d;
}

float *getImagEigenvalues(void) {
    return e;
}

Matrix getD (void) {
    Matrix X;
    float **D;
    int i, j;

    X = new Matrix(n,n);
    D = X->getArray();

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            D[i][j] = 0.;
        }
        D[i][i] = d[i];
        if (e[i] > 0.) {
            D[i][i + 1] = e[i];
        } else if (e[i] < 0.) {
            D[i][i - 1] = e[i];
        }
    }

    X->setArray(D);

    return X;
}

static void create(Matrix matrix) {
    float **A;
    int i, j;

    A = matrix->getArray();
    n = matrix->getColumnDimension();
    V = ({ });
    H = ({ });
    for (i = 0; i < n; i++) {
        V += ({ allocate_float(n) });
        H += ({ allocate_float(n) });
    }
    ort = allocate_float(n);
    d = allocate_float(n);
    e = allocate_float(n);
    issymmetric = TRUE;

    for (j = 0; (j < n) & issymmetric; j++) {
        for (i = 0; (i < n) & issymmetric; i++) {
            issymmetric = (A[i][j] == A[j][i]);
        }
    }

    if (issymmetric) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                V[i][j] = A[i][j];
            }
        }

        /* Tridiagonalize. */
        tred2();

        /* Diagonalize. */
        tql2();
    } else {
        for (j = 0; j < n; j++) {
            for (i = 0; i < n; i++) {
                H[i][j] = A[i][j];
            }
        }

        /* Reduce to Hessenberg form. */
        orthes();

        /* Reduce Hessenberg to real Schur form. */
        hqr2();
    }
}
