#include <Function.h>
#include <Number.h>
#include <type.h>

inherit Function;

private Number target;

static create(mixed target) {
    int target_type;

    target_type = typeof(target);
    if (target_type == T_OBJECT && target <- Number) {
        ::target = target;
        return;
    }

    ::target = new Number(target);
}

int evaluate(mixed value) {
    int value_type;
    Number n;

    value_type = typeof(value);
    if (value_type == T_INT || value_type == T_FLOAT) {
        n = new Number(value);
        return n > target;
    }

    if (value_type == T_OBJECT && value <- Number) {
        return n > target;
    }
    
    return FALSE;
}
