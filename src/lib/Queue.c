#include <Continuation.h>

private mixed *queue;
private Continuation *suspended;

static void create(void) {
    queue = ({ });
    suspended = ({ });
}

Continuation enqueueCont(void) {
    return new ChainedContinuation("enqueue");
}

static void enqueue(mixed value) {
    Continuation token;

    if (sizeof(suspended) != 0) {
        token = suspended[0];
        suspended = suspended[1 ..];
        token->runNext(value);
    } else {
        queue += ({ value });
    }
}

Continuation dequeueCont(void) {
    return new Continuation("dequeue");
}

static mixed dequeue(void) {
    mixed value;

    if (sizeof(queue) != 0) {
        value = queue[0];
        queue = queue[1 ..];
        return value;
    }
    suspended += ({ suspendContinuation() });
    return nil;
}
