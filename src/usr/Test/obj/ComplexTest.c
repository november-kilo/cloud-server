#include <Test.h>

inherit Test;

private Complex z;

private void createFromCartesian(void) {
    z = new Complex(3., 5.);

    expectEqual(TEST_LINE, "(3 + 5 i)", z->toString());
    expectEqual(TEST_LINE, 3., z->re());
    expectEqual(TEST_LINE, 5., z->im());
}

private void createFromPolarWhenRLessThanZero(void) {
    z = new Complex(-3., pi() / 2.);

    expectEqual(TEST_LINE, "(-3 + 1.57079633 i)", z->toString());
    expectEqual(TEST_LINE, -3., z->re());
    expectEqual(TEST_LINE, 1.57079633, z->im());
}

private void createFromPolarWhenRNotLessThanZero(void) {
    z = new Complex(3., pi() / 2.);

    expectEqual(TEST_LINE, "(3 + 1.57079633 i)", z->toString());
    expectEqual(TEST_LINE, 3., z->re());
    expectEqual(TEST_LINE, 1.57079633, z->im());
}

private void complexConjugate(void) {
    z = new Complex(3., 5.);

    z = z->conjugate();

    expectEqual(TEST_LINE, "(3 + -5 i)", z->toString());
    expectEqual(TEST_LINE, 3., z->re());
    expectEqual(TEST_LINE, -5., z->im());
}

private void division(void) {
    Complex a, b;
    a = new Complex(3., 5.);
    b = new Complex(5., 3.);

    z = a / b;

    expectEqual(TEST_LINE, "(0.882352941 + 0.470588235 i)", z->toString());

    z = b / a;

    expectEqual(TEST_LINE, "(0.882352941 + -0.470588235 i)", z->toString());
}

private void divisionByZero(void) {
    Complex a, b;
    a = new Complex(3., 5.);
    b = new Complex(0., 0.);

    expectEqual(TEST_LINE, "Complex: division by zero.", catch(a / b));
}

void runTests(void) {
    createFromCartesian();
    createFromPolarWhenRLessThanZero();
    createFromPolarWhenRNotLessThanZero();
    complexConjugate();
    division();
    divisionByZero();
}