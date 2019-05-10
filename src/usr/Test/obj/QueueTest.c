#include <Test.h>

inherit Test;

private Queue queue;

#define EMPTY_QUEUE "([ \"meta\":([ \"queue\":0, \"suspended\":0 ]), \"queue\":({ }), \"suspended\":({ }) ])"
#define ONE_VALUE_ENQUEUED "Jabberwocky"

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
    queryQueueState(TEST_LINE, EMPTY_QUEUE);
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
}
