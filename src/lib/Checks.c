#include <NKlib.h>

private int objectHasEquals(mixed arg) {
    return T_OBJECT == typeof(arg) && function_object("equals", arg);
}

mixed objectEquals(mixed left, mixed right, varargs float tolerance) {
    if (objectHasEquals(left)) {
        return left->equals(right);
    }
    return nil;
}
