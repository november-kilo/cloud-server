#include <Test.h>

inherit Test;

private Matrix matrix;
private float **square;
private float **symmetric;
private float **asymmetric;

void runBeforeTests(void) {
    square = ({
        ({ 163., 188., 212. }),
        ({ 187., 214., 242. }),
        ({ 210., 240., 270. })
    });

    symmetric = ({
        ({ 5., 6., 7. }),
        ({ 6., 3., 2. }),
        ({ 7., 2., 1. })
    });

    asymmetric = ({
        ({ 4., 8., 1., 2. }),
        ({ 9., 2., 3., 4. }),
        ({ 1., 3., 6., 4. }),
        ({ 2., 4., 3., 5. })
    });
}

private void createMatrixWithNoOptionalArg(void) {
    matrix = new Matrix(3, 2);

    expectEqual(TEST_LINE,
        "({ ({ 0.0, 0.0 }), ({ 0.0, 0.0 }), ({ 0.0, 0.0 }) })",
        matrix->toString());
}

private void createMatrixWithOptionalArg(void) {
    matrix = new Matrix(3, 2, 2.);

    expectEqual(TEST_LINE,
        "({ ({ 2.0, 2.0 }), ({ 2.0, 2.0 }), ({ 2.0, 2.0 }) })",
        matrix->toString());
}

private void createMatrixInvalid(void) {
    expectEqual(TEST_LINE,
        "Matrix: cannot create matrix with such dimensions.",
        catch(new Matrix(0, 1)));

    expectEqual(TEST_LINE,
        "Matrix: cannot create matrix with such dimensions.",
        catch(new Matrix(1, 0)));
}

private void setMatrix(void) {
    matrix = new Matrix(3, 3);

    matrix->setMatrix(square);

    expectEqual(TEST_LINE,
        "({ ({ 163.0, 187.0, 210.0 }), ({ 188.0, 214.0, 240.0 }), ({ 212.0, 242.0, 270.0 }) })",
        matrix->transpose()->toString());
}

private void transposeMatrix(void) {
    matrix = new Matrix(3, 3);
    matrix->setMatrix(square);

    matrix = matrix->transpose();

    expectEqual(TEST_LINE,
        "({ ({ 163.0, 188.0, 212.0 }), ({ 187.0, 214.0, 242.0 }), ({ 210.0, 240.0, 270.0 }) })",
        matrix->transpose()->toString());
}

private void matrixProduct(void) {
    matrix = new Matrix(3, 3);
    matrix->setMatrix(square);

    matrix = matrix->times(matrix->transpose());

    expectEqual(TEST_LINE,
        "({ ({ 106857.0, 122017.0, 136590.0 }), " +
        "({ 122017.0, 139329.0, 155970.0 }), " +
        "({ 136590.0, 155970.0, 174600.0 }) })",
        matrix->toString());
}

private void eigenvalueDecompositionSymmetric(void) {
    EigenvalueDecomposition eig;
    float *r, *i;

    matrix = new Matrix(3, 3);
    matrix->setMatrix(symmetric);
    eig = matrix->eig();

    r = eig->getRealEigenvalues();
    i = eig->getImagEigenvalues();

    expectEqual(TEST_LINE,
        "({ -5.0, 0.291796068, 13.7082039 })",
        dump_value(r, ([ ])));
    expectEqual(TEST_LINE,
        "({ 0.0, 0.0, 0.0 })",
        dump_value(i, ([ ])));
}

private void eigenvalueDecompositionAsymmetric(void) {
    EigenvalueDecomposition eig;
    float *r, *i;

    matrix = new Matrix(4, 4);
    matrix->setMatrix(asymmetric);
    eig = matrix->eig();

    r = eig->getRealEigenvalues();
    i = eig->getImagEigenvalues();

    expectEqual(TEST_LINE,
        "({ 15.4152584, -5.93194655, 1.76692268, 5.74976548 })",
        dump_value(r, ([ ])));
    expectEqual(TEST_LINE,
        "({ 0.0, 0.0, 0.0, 0.0 })",
        dump_value(i, ([ ])));
}

void runTests(void) {
    createMatrixWithNoOptionalArg();
    createMatrixWithOptionalArg();
    createMatrixInvalid();
    setMatrix();
    transposeMatrix();
    matrixProduct();
    eigenvalueDecompositionSymmetric();
    eigenvalueDecompositionAsymmetric();
}
