#include <Math.h>

private float **A;
private int m;
private int n;

float **getArray(void) {
    return A;
}

int getRowDimension(void) {
    return m;
}

int getColumnDimension(void) {
    return n;
}

string toString(void) {
    return dump_value(A);
}

void setIJ(int i, int j, float s) {
    A[i][j] = s;
}

Matrix transpose(void) {
    Matrix X;
    float **C;
    int i, j;

    X = new Matrix(n, m);
    C = X->getArray();
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            C[j][i] = A[i][j];
        }
    }

    X->setMatrix(C);

    return X;
}

Matrix times(Matrix B) {
    Matrix X;
    float **C;
    float *Arowi, *Bcolj;
    float s;
    int i, j, k;

    if (B->getRowDimension() != n) {
        error("Matrix inner dimensions must agree.");
    }

    X = new Matrix(m, B->getColumnDimension());
    C = X->getArray();
    Bcolj = allocate_float(n);

    for (j = 0; j < B->getColumnDimension(); j++) {
        for (k = 0; k < n; k++) {
            Bcolj[k] = B->getArray()[k][j];
        }
        for (i = 0; i < m; i++) {
            Arowi = A[i];
            s = 0.;
            for (k = 0; k < n; k++) {
                s += Arowi[k] * Bcolj[k];
            }
            C[i][j] = s;
        }
    }
    return X;
}

void balance(void) {
    float radix, squareRadix, f, g, r, c, s;
    int completed, i, j;

    radix = (float) FLT_RADIX;
    squareRadix = radix * radix;
    completed = FALSE;

    while (!completed) {
        completed = TRUE;

        for (i = 0;i < n; i++) {
            r = 0.;
            c = 0.;

            for (j = 0; j < n; j++) {
                if (j != i) {
                    c += fabs(A[j][i]);
                    r += fabs(A[i][j]);
                }
            }

            if (c != 0. && r != 0.) {
                g = r / radix;
                f = 1.;
                s = c + r;

                while (c < g) {
                    f *= radix;
                    c *= squareRadix;
                }

                g = r * radix;
                while (c > g) {
                    f /= radix;
                    c /= squareRadix;
                }

                if ((c + r) / f < 0.95 * s) {
                    completed = FALSE;
                    g = 1. / f;

                    for (j = 0; j < n; j++) {
                        A[i][j] *= g;
                    }

                    for (j = 0; j < n; j++) {
                        A[j][i] *= f;
                    }
                }
            }
        }
    }
}

EigenvalueDecomposition eig(void) {
    return new EigenvalueDecomposition((Matrix) this_object());
}

void setMatrix(float **A) {
    ::A = A;
}

static void create(int m, int n, varargs float s) {
    int i, j;

    if (m < 1 || n < 1) {
        error("Matrix: cannot create matrix with such dimensions.");
    }
    ::m = m;
    ::n = n;
    A = ({ });
    for (i = 0; i < m; i++) {
        A += ({ allocate_float(n) });
        if (s) {
            for (j = 0; j < n; j++) {
                A[i][j] = s;
            }
        }
    }
}
