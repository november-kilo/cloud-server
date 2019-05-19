#include <Test.h>

inherit Test;

private Vector v;

private void componentsReturnsTheComponents(void) {
    Number *actual;

    actual = v->components();

    expectEqual(TEST_LINE, 1., actual[0]->toFloat());
    expectEqual(TEST_LINE, 2., actual[1]->toFloat());
    expectEqual(TEST_LINE, 3., actual[2]->toFloat());
}

private void sizeReportsExpectedValue(void) {
    expectEqual(TEST_LINE, 3, v->size());
}

private void getShouldReturnNilForNullVector(void) {
    Vector x;

    x = new Vector(({ }));

    expectNil(TEST_LINE, x->get(-1));
    expectNil(TEST_LINE, x->get(0));
    expectNil(TEST_LINE, x->get(1));
}

private void getComponentByIndex(void) {
    expectEqual(TEST_LINE, "Vector: Invalid index.", catch(v->get(-1)));
    expectEqual(TEST_LINE, 1., v->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., v->get(1)->toFloat());
    expectEqual(TEST_LINE, 3., v->get(2)->toFloat());
    expectEqual(TEST_LINE, "Vector: Invalid index.", catch(v->get(4)));
}

private void additionElementwise(void) {
    Vector actual;

    actual = v + new Number(2);

    expectEqual(TEST_LINE, 3., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 4., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 5., actual->get(2)->toFloat());
}

private void additionVector(void) {
    Vector actual, y, nullVector;

    y = new Vector(({ new Number(3), new Number(4), new Number(5) }));
    nullVector = new Vector(({ }));

    actual = v + y;

    expectEqual(TEST_LINE, 4., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 6., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 8., actual->get(2)->toFloat());

    actual = y + v;

    expectEqual(TEST_LINE, 4., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 6., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 8., actual->get(2)->toFloat());

    actual = v + nullVector;

    expectEqual(TEST_LINE, 1., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 3., actual->get(2)->toFloat());

    actual = nullVector + v;

    expectEqual(TEST_LINE, 1., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 3., actual->get(2)->toFloat());
}

private void additionErrors(void) {
    Vector y;

    y = new Vector(({ new Number(4) }));

    expectEqual(TEST_LINE, "Vector: invalid operand for addition.", catch(v + 4));
    expectEqual(TEST_LINE, "Vector: size mismatch for addition.", catch(v + y));
}

private void subtractionElementwise(void) {
    Vector actual;

    actual = v - new Number(2);

    expectEqual(TEST_LINE, -1., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 0., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 1., actual->get(2)->toFloat());
}

private void subtractionVector(void) {
    Vector y, nullVector;
    Vector actual;

    y = new Vector(({ new Number(3), new Number(4), new Number(5) }));
    nullVector = new Vector(({ }));

    actual = v - y;

    expectEqual(TEST_LINE, -2., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, -2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, -2., actual->get(2)->toFloat());

    actual = y - v;

    expectEqual(TEST_LINE, 2., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(2)->toFloat());

    actual = v - nullVector;

    expectEqual(TEST_LINE, 1., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 3., actual->get(2)->toFloat());

    actual = nullVector - v;

    expectEqual(TEST_LINE, 1., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 2., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 3., actual->get(2)->toFloat());
}

private void subtractionErrors(void) {
    Vector y;

    y = new Vector(({ new Number(4) }));

    expectEqual(TEST_LINE, "Vector: invalid operand for subtraction.", catch(v - 4));
    expectEqual(TEST_LINE, "Vector: size mismatch for subtraction.", catch(v - y));
}

private void multiplicationElementwise(void) {
    Vector actual;

    actual = v * new Number(2);

    expectEqual(TEST_LINE, 2., actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 4., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 6., actual->get(2)->toFloat());
}

private void multiplicationErrors(void) {
    Vector y;

    y = new Vector(({ new Number(4) }));

    expectEqual(TEST_LINE, "Vector: invalid operand for multiplication.", catch(v * 4));
    expectEqual(TEST_LINE, "Vector: vector product defined only for 3D vectors.", catch(v * y));
}

private void divisionElementwise(void) {
    Vector actual;

    actual = v / new Number(2);

    expectEqual(TEST_LINE, 0.5, actual->get(0)->toFloat());
    expectEqual(TEST_LINE, 1., actual->get(1)->toFloat());
    expectEqual(TEST_LINE, 1.5, actual->get(2)->toFloat());
}

private void divisionErrors(void) {
    Vector y;

    y = new Vector(({ new Number(4) }));

    expectEqual(TEST_LINE, "Vector: invalid operand for division.", catch(v / 4));
    expectEqual(TEST_LINE, "Vector: vector division is not uniquely defined.", catch(v / y));
}

private void negation(void) {
    Vector nullVector, a;

    nullVector = new Vector(({ }));
    a = -v;

    expectEqual(TEST_LINE, 0, nullVector->size());

    expectEqual(TEST_LINE, -1., a->get(0)->toFloat());
    expectEqual(TEST_LINE, -2., a->get(1)->toFloat());
    expectEqual(TEST_LINE, -3., a->get(2)->toFloat());
}

private void vectorProperties(void) {
    Vector a, b, c, d, zeroVector;
    Number m, n;

    a = new Vector(({ new Number(3), new Number(5) }));
    b = new Vector(({ new Number(4), new Number(6) }));
    c = new Vector(({ new Number(7), new Number(8) }));
    zeroVector = new Vector(({ }));
    m = new Number(2);
    n = new Number(3);

    d = a + (b + c);

    expectEqual(TEST_LINE, 14., d->get(0)->toFloat());
    expectEqual(TEST_LINE, 19., d->get(1)->toFloat());

    d = (a + b) + c;

    expectEqual(TEST_LINE, 14., d->get(0)->toFloat());
    expectEqual(TEST_LINE, 19., d->get(1)->toFloat());

    c = a + (-a);

    expectEqual(TEST_LINE, 0., c->get(0)->toFloat());
    expectEqual(TEST_LINE, 0., c->get(1)->toFloat());

    c = (a + b) * m;

    expectEqual(TEST_LINE, 14., c->get(0)->toFloat());
    expectEqual(TEST_LINE, 22., c->get(1)->toFloat());

    c = a * m + b * m;

    expectEqual(TEST_LINE, 14., c->get(0)->toFloat());
    expectEqual(TEST_LINE, 22., c->get(1)->toFloat());
}

void runBeforeTests(void) {
    v = new Vector(({ new Number(1.), new Number(2.), new Number(3.) }));
}

void runTests(void) {
    componentsReturnsTheComponents();
    sizeReportsExpectedValue();
    getShouldReturnNilForNullVector();
    getComponentByIndex();
    additionElementwise();
    additionVector();
    additionErrors();
    subtractionElementwise();
    subtractionVector();
    subtractionErrors();
    multiplicationElementwise();
    multiplicationErrors();
    divisionElementwise();
    divisionErrors();
    negation();
    vectorProperties();
}
