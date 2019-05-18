#include <Test.h>

inherit Test;

private Checks checks;
private Random random;
private Number number;

private void objectEqualsShouldIndicateWhetherOneEqualsAnother(void) {
    string str;
    int i;
    float f;

    str = random->randomString(42);
    i = random->randomInt() % 42;
    number = new Number(i);

    expectTrue(TEST_LINE, checks->objectEquals(number, number));
    expectNil(TEST_LINE, checks->objectEquals(i, i));
    expectNil(TEST_LINE, checks->objectEquals((float) i, (float) i));
    expectNil(TEST_LINE, checks->objectEquals(str, str));

    expectFalse(TEST_LINE, checks->objectEquals(number, new Number(1)));
    expectTrue(TEST_LINE, checks->objectEquals(number, new Number(i)));
    expectNil(TEST_LINE, checks->objectEquals(i, i + 1));
    expectNil(TEST_LINE, checks->objectEquals((float) i, (float) i + 0.5));
    expectNil(TEST_LINE, checks->objectEquals(str, "not " + str));
    expectNil(TEST_LINE, checks->objectEquals(str, i));
    expectNil(TEST_LINE, checks->objectEquals(str, (float) i));
    expectNil(TEST_LINE, checks->objectEquals(str, number));
    expectNil(TEST_LINE, checks->objectEquals(i, (float) i));
    expectNil(TEST_LINE, checks->objectEquals((float) i, i));
    expectFalse(TEST_LINE, checks->objectEquals(number, str));
    expectTrue(TEST_LINE, checks->objectEquals(number, i));
    expectTrue(TEST_LINE, checks->objectEquals(number, (float) i));

    expectFalse(TEST_LINE, checks->objectEquals(number, ({ })));
    expectFalse(TEST_LINE, checks->objectEquals(number, ([ ])));
    expectNil(TEST_LINE, checks->objectEquals(i, ({ })));
    expectNil(TEST_LINE, checks->objectEquals(i, ([ ])));
    expectNil(TEST_LINE, checks->objectEquals((float) i, ({ })));
    expectNil(TEST_LINE, checks->objectEquals((float) i, ([ ])));
    expectNil(TEST_LINE, checks->objectEquals(str, ({ })));
    expectNil(TEST_LINE, checks->objectEquals(str, ([ ])));
}

void runBeforeTests(void) {
    checks = new Checks();
    random = new Random();
}

void runTests(void) {
    objectEqualsShouldIndicateWhetherOneEqualsAnother();
}
