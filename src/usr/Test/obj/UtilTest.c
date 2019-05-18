#include <Test.h>

inherit Test;
inherit NK_KFUN;

private Terminal terminal;

private void piShouldReturnExpectedValue(void) {
    expectEqual(TEST_LINE, 3.14159265, pi());
}

private void minShouldReturnMinimumValue(void) {
    Rational r1, r2;

    r1 = new Rational(0.2);
    r2 = new Rational(0.4);

    expectEqual(TEST_LINE, 1, min(2, 1));
    expectEqual(TEST_LINE, 1, min(1, 2));
    expectEqual(TEST_LINE, 1., min(2., 1.));
    expectEqual(TEST_LINE, 1., min(1., 2.));
    expectEqual(TEST_LINE, "ABC", min("ABC", "abc"));
    expectEqual(TEST_LINE, "ABC", min("abc", "ABC"));
    expectTrue(TEST_LINE, r1->equals(min(r1, r2)));
    expectTrue(TEST_LINE, r1->equals(min(r2, r1)));
}

private void maxShouldReturnMaximumValue(void) {
    Rational r1, r2;

    r1 = new Rational(0.2);
    r2 = new Rational(0.4);

    expectEqual(TEST_LINE, 2, max(2, 1));
    expectEqual(TEST_LINE, 2, max(1, 2));
    expectEqual(TEST_LINE, 2., max(2., 1.));
    expectEqual(TEST_LINE, 2., max(1., 2.));
    expectEqual(TEST_LINE, "abc", max("ABC", "abc"));
    expectEqual(TEST_LINE, "abc", max("abc", "ABC"));
    expectTrue(TEST_LINE, r2->equals(max(r1, r2)));
    expectTrue(TEST_LINE, r2->equals(max(r2, r1)));
}

private void colors256ShouldThrowExpectedError(void) {
    expectEqual(TEST_LINE, "Terminal: Invalid color.", catch(terminal->color256(-1, "foo")));
    expectEqual(TEST_LINE, "Terminal: Invalid color.", catch(terminal->color256(256, "foo")));
}

private void hypotShouldReturnExpectedValue(void) {
    expectEqual(TEST_LINE, 5., hypot(4., 3.));
    expectEqual(TEST_LINE, 5., hypot(3., 4.));
    expectEqual(TEST_LINE, 5.65685425, hypot(4., 4.));
    expectEqual(TEST_LINE, 0., hypot(0., 0.));
    expectEqual(TEST_LINE, 3., hypot(3., 0.));
    expectEqual(TEST_LINE, 3., hypot(0., 3.));
}

void runBeforeTests(void) {
    terminal = new Terminal();
}

void runTests(void) {
    piShouldReturnExpectedValue();
    minShouldReturnMinimumValue();
    maxShouldReturnMaximumValue();
    colors256ShouldThrowExpectedError();
    hypotShouldReturnExpectedValue();
}
