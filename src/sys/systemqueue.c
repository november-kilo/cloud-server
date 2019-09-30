#include <Queue.h>

inherit Queue;

Continuation publisher(void) {
	return enqueueCont();
}

Continuation consumer(void) {
	return dequeueCont();
}
