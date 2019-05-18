#include <Test.h>

inherit Test;

private Checks checks;
private Random random;
private Number number;

private void checkEqualsShouldIndicateWhetherOneEqualsAnother(void) {
    string str;
    int i;
    float f;

    str = random->randomString(42);
    i = random->randomInt() % 42;
    number = new Number(i);

    expectTrue(TEST_LINE, checks->checkEquals(number, number));
    expectTrue(TEST_LINE, checks->checkEquals(i, i));
    expectTrue(TEST_LINE, checks->checkEquals((float) i, (float) i));
    expectTrue(TEST_LINE, checks->checkEquals(str, str));

    expectFalse(TEST_LINE, checks->checkEquals(number, new Number(1)));
    expectFalse(TEST_LINE, checks->checkEquals(i, i + 1));
    expectFalse(TEST_LINE, checks->checkEquals((float) i, (float) i + 0.5));
    expectFalse(TEST_LINE, checks->checkEquals(str, "not " + str));
    expectFalse(TEST_LINE, checks->checkEquals(str, i));
    expectFalse(TEST_LINE, checks->checkEquals(str, (float) i));
    expectFalse(TEST_LINE, checks->checkEquals(str, number));
    expectFalse(TEST_LINE, checks->checkEquals(i, (float) i));
    expectFalse(TEST_LINE, checks->checkEquals(number, str));
    expectFalse(TEST_LINE, checks->checkEquals(number, i));
    expectFalse(TEST_LINE, checks->checkEquals(number, (float) i));

    expectFalse(TEST_LINE, checks->checkEquals(number, ({ })));
    expectFalse(TEST_LINE, checks->checkEquals(number, ([ ])));
    expectFalse(TEST_LINE, checks->checkEquals(i, ({ })));
    expectFalse(TEST_LINE, checks->checkEquals(i, ([ ])));
    expectFalse(TEST_LINE, checks->checkEquals((float) i, ({ })));
    expectFalse(TEST_LINE, checks->checkEquals((float) i, ([ ])));
    expectFalse(TEST_LINE, checks->checkEquals(str, ({ })));
    expectFalse(TEST_LINE, checks->checkEquals(str, ([ ])));

    expectTrue(TEST_LINE, checks->checkEquals(number, new Number(i))); /* TODO: fix */
}

void runBeforeTests(void) {
    checks = new Checks();
    random = new Random();
}

void runTests(void) {
    checkEqualsShouldIndicateWhetherOneEqualsAnother();
}
