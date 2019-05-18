#include <Test.h>

inherit Test;

private Number number;

private void checkNumberReturnsExpectedValue(void) {
    number = new Number(0.8);

    expectEqual(TEST_LINE, "0.8", number->toString());
    expectEqual(TEST_LINE, 1, number->toInt());
    expectEqual(TEST_LINE, 0, new Number(0.2)->toInt());
    expectEqual(TEST_LINE, 0.8, number->toFloat());
    expectTrue(TEST_LINE, new Rational(4, 5)->equals(number->toRational()));
}

private void intIsNumber(void) {
    expectFalse(TEST_LINE, new Number(-1)->isNan());
    expectFalse(TEST_LINE, new Number(0)->isNan());
    expectFalse(TEST_LINE, new Number(1)->isNan());
}

private void floatIsNumber(void) {
    expectFalse(TEST_LINE, new Number(-1.)->isNan());
    expectFalse(TEST_LINE, new Number(0.)->isNan());
    expectFalse(TEST_LINE, new Number(1.)->isNan());
}

private void numberIsNumber(void) {
    expectFalse(TEST_LINE, new Number(-1.)->isNan());
    expectFalse(TEST_LINE, new Number(0.)->isNan());
    expectFalse(TEST_LINE, new Number(1.)->isNan());
}

private void rationalIsNumber(void) {
    expectFalse(TEST_LINE, new Number(new Rational(pi()))->isNan());
}

private void otherIsNotNumber(void) {
    expectTrue(TEST_LINE, new Number(({ }))->isNan());
    expectTrue(TEST_LINE, new Number(([ ]))->isNan());
    expectTrue(TEST_LINE, new Number(new Array(({ })))->isNan());
}

private void castsToString(void) {
    expectEqual(TEST_LINE, "-1", new Number(-1)->toString());
    expectEqual(TEST_LINE, "0", new Number(0)->toString());
    expectEqual(TEST_LINE, "1", new Number(1)->toString());
    expectEqual(TEST_LINE, "-1", new Number(-1.)->toString());
    expectEqual(TEST_LINE, "0", new Number(0.)->toString());
    expectEqual(TEST_LINE, "1", new Number(1.)->toString());
    expectEqual(TEST_LINE, "-1.1", new Number(-1.1)->toString());
    expectEqual(TEST_LINE, "0.1", new Number(0.1)->toString());
    expectEqual(TEST_LINE, "1.1", new Number(1.1)->toString());
    expectEqual(TEST_LINE, "3.14159265", new Number(pi())->toString());
}

void runTests(void) {
    checkNumberReturnsExpectedValue();
    intIsNumber();
    floatIsNumber();
    rationalIsNumber();
    otherIsNotNumber();
    castsToString();
}
