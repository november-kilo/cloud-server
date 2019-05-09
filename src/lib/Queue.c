#include <Continuation.h>

private mixed *queue;
private ContinuationToken *suspended;

static void create(void) {
    queue = ({ });
    suspended = ({ });
}

mapping state(void) {
    return ([
        "meta": ([
            "queue": sizeof(queue),
            "suspended": sizeof(suspended)
        ]),
        "queue": queue,
        "suspended": suspended
    ]);
}

Continuation enqueueCont(void) {
    return new ChainedContinuation("enqueue");
}

static void enqueue(mixed value) {
    ContinuationToken token;

    if (sizeof(suspended) != 0) {
        token = suspended[0];
        suspended = suspended[1 ..];
        token->resumeContinuation(value);
    } else {
        queue += ({ value });
    }
}

Continuation dequeueCont(void) {
    return new Continuation("dequeue");
}

static mixed dequeue() {
    mixed value;

    if (sizeof(queue) != 0) {
        value = queue[0];
        queue = queue[1 ..];
        return value;
    }
    suspended += ({ suspendContinuation() });
    return nil;
}
