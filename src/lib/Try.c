/* Compose functions that can throw, short-circuit on error. */

#include <Try.h>

private Result result;

static void create(Result result) {
    ::result = result;
}

Result result() {
    return result;
}

string fold() {
    return result->fold();
}

/*
 * Chains a call to obj->func(previousValue, args...)
 * and returns a new Try.
 * Catches any error raised by the call into a failed Result.
 */
Try map(object obj, string func, mixed args...) {
    Result r;

    if (!result->ok()) {
        return new Try(result); /* or just this_object() */
    }

    if (!function_object(func, obj)) {
        return TRY_FAILURE("Not a function: " + func);
    }

    try {
        r = RESULT_SUCCESS(call_other(obj, func, (({ result->value() }) + args)...));
    } catch (err) {
        r = RESULT_FAILURE(err);
    }

    return new Try(r);
}
