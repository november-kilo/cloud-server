#include <Continuation.h>

private mixed *queue;
private ContinuationToken *suspended;

static void create(void) {
    queue = ({ });
    suspended = ({ });
}

mapping state(void) {
    return ([ "queue": sizeof(queue), "suspended": sizeof(suspended) ]);
}

Continuation enqueueCont(mixed obj) {
    return new Continuation("enqueue", obj);
}

static void enqueue(mixed obj) {
    ContinuationToken token;

    if (sizeof(suspended) != 0) {
        token = suspended[0];
        suspended = suspended[1 ..];
        token->resumeContinuation(obj);
    } else {
        queue += ({ obj });
    }
}

Continuation dequeueCont(void) {
    return new Continuation("dequeue");
}

static mixed dequeue() {
    mixed obj;

    if (sizeof(queue) != 0) {
        obj = queue[0];
        queue = queue[1 ..];
        return obj;
    }
    suspended += ({ suspendContinuation() });
    return nil;
}
