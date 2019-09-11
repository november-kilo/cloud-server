#include <Array.h>
#include <Function.h>
#include <Iterator.h>
#include <Sort.h>

inherit Iterable;
private inherit "/lib/util/string";

private mixed *elements;
private int iterationCount;

static void create(mixed *elements) {
    ::elements = elements;
}

string toString(void) {
    return dump_value(elements, ([ ]));
}

mixed *getArray(void) {
    return elements;
}

int size(void) {
    return sizeof(elements);
}

mixed get(int index) {
    if (index < 0 || index >= size()) {
        error("Array: index " + index + " is out of range");
    }

    return elements[index];
}

private int *fixFromTo(int from, int to) {
    if (!from) {
        from = 0;
    }

    if (!to) {
        to = size() - 1;
    }

    return ({ from, to });
}

void each(Function eachFunction, varargs int from, int to) {
    Iterator iterator;
    int i;
    mixed m;

    ({ from, to }) = fixFromTo(from, to);
    iterator = new IntIterator(from, to);

    while (!iterator->end()) {
        i = iterator->next();
        m = get(i);
        eachFunction->evaluate(m, i);
    }
}

Array filter(Function filterer, varargs int from, int to) {
    Iterator iterator;
    Array filtered;
    mixed element;
    int i, fCount;

    ({ from, to }) = fixFromTo(from, to);
    iterator = iterator(from, to);

    while (!iterator->end()) {
        element = iterator->next();
        if (filterer->evaluate(element)) {
            fCount++;
        }
    }

    filtered = new Array(allocate(fCount));
    iterator->reset();
    i = 0;

    while (!iterator->end()) {
        element = iterator->next();
        if (filterer->evaluate(element)) {
            filtered[i++] = element;
        }
    }

    return filtered;
}

Array map(Function mapper, varargs int from, int to) {
    Iterator iterator;
    Array mapped;
    mixed element;
    int i;

    ({ from, to }) = fixFromTo(from, to);
    iterator = iterator(from, to);
    mapped = new Array(allocate(to - from + 1));
    i = 0;

    while (!iterator->end()) {
        element = iterator->next();
        mapped[i] = mapper->evaluate(element);
        i++;
    }

    return mapped;
}

private int isLastIteration(int from, int to) {
    return iterationCount == to - from + 1;
}

mixed reduce(Function reducer, varargs int from, int to, int evalFirstVal) {
    Iterator iterator;
    mixed nextValue, currentValue;

    ({ from, to }) = fixFromTo(from, to);
    iterator = new Iterator(this_object(), from, to);
    currentValue = iterator->next();
    if (evalFirstVal) {
        currentValue = reducer->evaluate(nil, currentValue, isLastIteration(from, to));
    }

    while (!iterator->end()) {
        nextValue = iterator->next();
        currentValue = reducer->evaluate(nextValue, currentValue, isLastIteration(from, to));
    }

    return currentValue;
}

string toList(void) {
    return reduce(new ArrayToListReducer(), 0, size() - 1, 1);
}

void sort(void) {
    new MergeSort()->sort(elements);
}

static mixed operator[] (int index) {
    return get(index);
}

static Array operator[..] (mixed from, mixed to) {
    if (from == nil) {
        from = 0;
    }
    
    if (to == nil) {
        to = size() - 1;
    }
    
    return new Array(elements[from..to]);
}

static void operator[]= (int index, mixed value) {
    elements[index] = value;
}

Array union(Array x) {
    return new Array(getArray() | x->getArray());
}

Array intersection(Array x) {
    return new Array(getArray() & x->getArray());
}

Array symmetricDifference(Array x) {
    return new Array(getArray() ^ x->getArray());
}

Array difference(Array x) {
    return intersection(symmetricDifference(x));
}

static Array operator| (Array x) {
    return this_object()->union(x);
}

static Array operator& (Array x) {
    return this_object()->intersection(x);
}

static Array operator^ (Array x) {
    return this_object()->symmetricDifference(x);
}

static Array operator- (Array x) {
    return this_object()->difference(x);
}

int isSubset(Array x) {
    
}

mixed iteratorStart(mixed from, mixed to) {
    return ({ from, to, from, 0 });
}

mixed *iteratorNext(mixed state) {
    int from, to, index;
    mixed value;

    ({ from, to, index, iterationCount }) = state;
    value = elements[index];
    index++;
    iterationCount++;

    return ({ ({ from, to, index, iterationCount }), value });
}

int iteratorEnd(mixed state) {
    int from, to, index;

    ({ from, to, index, iterationCount }) = state;

    return isLastIteration(from, to);
}
