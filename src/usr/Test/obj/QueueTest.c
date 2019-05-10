#include <Test.h>

inherit Test;

private Queue queue;

#define EMPTY_QUEUE "([ \"meta\":([ \"queue\":0, \"suspended\":0 ]), \"queue\":({ }), \"suspended\":({ }) ])"
#define ONE_VALUE_ENQUEUED "Jabberwocky"

static int enqueueValue(int value) {
    return value;
}

private void queryQueueState(string testLine, string expected) {
    expectEqual(testLine, expected, dump_value(queue->state(), ([ ])));
}

private void enqueueContShouldReturnChainedContinuation(void) {
    expectTrue(TEST_LINE, queue->enqueueCont() <- ChainedContinuation);
}

private void dequeueContShouldReturnContinuation(void) {
    expectTrue(TEST_LINE, queue->dequeueCont() <- Continuation);
}

private void stateShouldIndicateQueueIsEmpty(void) {
    mapping state;

    state = queue->state();

    queryQueueState(TEST_LINE, EMPTY_QUEUE);
}

private void enqueueShouldAddToQueueWhenSuspendedIsEmpty(void) {
    Continuation continuation;
    int value;

    value = 42;
    continuation = new Continuation("enqueueValue", value);
    continuation >>= queue->enqueueCont();
    continuation += new Continuation("queryQueueState", TEST_LINE, ONE_VALUE_ENQUEUED);
    continuation->runNext();
}

void runBeforeTests(void) {
    if (!find_object(QUEUE_OBJ)) {
        compile_object(QUEUE_OBJ);
    }
    if (!queue) {
        queue = clone_object(QUEUE_OBJ);
    }
}

void runTests(void) {
    enqueueContShouldReturnChainedContinuation();
    dequeueContShouldReturnContinuation();
    stateShouldIndicateQueueIsEmpty();
    enqueueShouldAddToQueueWhenSuspendedIsEmpty();
}
