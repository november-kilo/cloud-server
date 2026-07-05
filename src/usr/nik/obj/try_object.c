/*
 * Object provides pure functions to Try.
 */

private int x;
private int initial_x;

int was_mutated() {
    return x != initial_x;
}

int initial_value() { return x; }

int add(int prevValue, varargs int y) {
    return prevValue + y;
}

int subtract(int prevValue, varargs int y) {
    return prevValue - y;
}

int multiply(int prevValue, varargs int y) {
    return prevValue * y;
}

int divide(int prevValue, varargs int y) {
    if (y == 0) {
        error("Division by zero");
    }

    return prevValue / y;
}

static create(int x) {
    ::x = x;
    ::initial_x = ::x;
}
