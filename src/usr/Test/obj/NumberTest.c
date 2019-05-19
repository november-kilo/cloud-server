#include <Test.h>

inherit Test;

private Number number;

private void checkNumberReturnsExpectedValue(void) {
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

private void equalsShouldIndicateEquality() {
    expectTrue(TEST_LINE, number->equals(new Number(0.8)));
    expectTrue(TEST_LINE, number->equals(number));
    expectTrue(TEST_LINE, number->equals(0.8));
    expectFalse(TEST_LINE, number->equals(0.79));
    expectFalse(TEST_LINE, number->equals(0.81));
    expectFalse(TEST_LINE, number->equals(0.801));

    expectTrue(TEST_LINE, number->equals(new Number("0.8")));
    expectTrue(TEST_LINE, number->equals(number));
    expectTrue(TEST_LINE, number->equals("0.8"));
    expectFalse(TEST_LINE, number->equals("0.79"));
    expectFalse(TEST_LINE, number->equals("0.81"));
    expectFalse(TEST_LINE, number->equals("0.801"));
}

private void operators(void) {
    expectEqual(TEST_LINE, 2.8, (number + 2)->toFloat());

    expectEqual(TEST_LINE, 1.2, (number + 0.4)->toFloat());
    expectEqual(TEST_LINE, 1.2, (number + new Number(0.4))->toFloat());
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number + ([])));

    expectEqual(TEST_LINE, 0.6, (number - 0.2)->toFloat());
    expectEqual(TEST_LINE, 0.6, (number - new Number(0.2))->toFloat());
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number - ([])));

    expectEqual(TEST_LINE, 0.32, (number * 0.4)->toFloat());
    expectEqual(TEST_LINE, 0.32, (number * new Number(0.4))->toFloat());
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number * ([])));

    expectEqual(TEST_LINE, 2., (number / 0.4)->toFloat());
    expectEqual(TEST_LINE, 2., (number / new Number(0.4))->toFloat());
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number / ([])));

    expectEqual(TEST_LINE, "Division by zero", catch(number / 0.));
    expectEqual(TEST_LINE, "Division by zero", catch(number / new Number(0.)));

    expectTrue(TEST_LINE, number > 0.79);
    expectFalse(TEST_LINE, number > 0.81);
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number > ([])));

    expectTrue(TEST_LINE, number >= 0.79);
    expectTrue(TEST_LINE, number >= 0.8);
    expectFalse(TEST_LINE, number >= 0.81);
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number >= ([])));

    expectFalse(TEST_LINE, number < 0.79);
    expectTrue(TEST_LINE, number < 0.81);
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number < ([])));

    expectFalse(TEST_LINE, number <= 0.79);
    expectTrue(TEST_LINE, number <= 0.8);
    expectTrue(TEST_LINE, number <= 0.81);
    expectEqual(TEST_LINE, "Number: invalid operand.", catch(number <= ([])));
}

private void negation(void) {
    expectEqual(TEST_LINE, -0.8, (-number)->toFloat());
}

private void absoluteValue(void) {
    expectEqual(TEST_LINE, 0.8, number->abs()->toFloat());

    number = -number;

    expectEqual(TEST_LINE, 0.8, number->abs()->toFloat());
}

void runBeforeTests(void) {
    number = new Number(0.8);
}

void runTests(void) {
    checkNumberReturnsExpectedValue();
    intIsNumber();
    floatIsNumber();
    rationalIsNumber();
    otherIsNotNumber();
    castsToString();
    equalsShouldIndicateEquality();
    operators();
    negation();
    absoluteValue();
}
