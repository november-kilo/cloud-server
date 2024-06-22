#include <Array.h>

inherit Iterable;
inherit "/lib/util/format";

private mixed *elements;
private int iteration_count;

static void create(mixed *elements) {
    ::elements = elements;
}

string stringify(void) {
    return ::to_string(elements);
}

mixed *get_array(void) {
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

private int *fix_from_to(int from, int to) {
    if (!from) {
	    from = 0;
    }

    if (!to) {
	    to = size() - 1;
    }

    return ({ from, to });
}

void each(Function each_fn, varargs int from, int to) {
    Iterator iterator;
    int i;
    mixed m;

    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);

    while (!iterator->end()) {
        i = iterator->next();
        m = get(i);
        each_fn->evaluate(m, i);
    }
}

Array filter(Function filterer, varargs int from, int to) {
    Iterator iterator;
    Array filtered;
    mixed element;
    int i, fCount;

    ({ from, to }) = fix_from_to(from, to);
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

    ({ from, to }) = fix_from_to(from, to);
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

private int is_last_iteration(int from, int to) {
    return iteration_count == to - from + 1;
}

mixed reduce(Function reducer, varargs int from, int to, int eval_first_val) {
    Iterator iterator;
    mixed next_value, current_value;

    ({ from, to }) = fix_from_to(from, to);
    iterator = new Iterator(this_object(), from, to);
    current_value = iterator->next();
    if (eval_first_val) {
	    current_value = reducer->evaluate(nil, current_value, is_last_iteration(from, to));
    }

    while (!iterator->end()) {
	    next_value = iterator->next();
	    current_value = reducer->evaluate(next_value, current_value, is_last_iteration(from, to));
    }

    return current_value;
}

void sort(void) {
    new MergeSort()->sort(elements);
}

static mixed operator [] (int index) {
    return get(index);
}

static Array operator [..] (mixed from, mixed to) {
    if (from == nil) {
	    from = 0;
    }

    if (to == nil) {
	    to = size() - 1;
    }

    return new Array(elements[from..to]);
}

static void operator []= (int index, mixed value) {
    elements[index] = value;
}

Array union(Array x) {
    return new Array(get_array() | x->get_array());
}

Array intersection(Array x) {
    return new Array(get_array() & x->get_array());
}

Array symmetric_difference(Array x) {
    return new Array(get_array() ^ x->get_array());
}

Array difference(Array x) {
    return intersection(symmetric_difference(x));
}

static Array operator | (Array x) {
    return this_object()->union(x);
}

static Array operator & (Array x) {
    return this_object()->intersection(x);
}

static Array operator ^ (Array x) {
    return this_object()->symmetric_difference(x);
}

static Array operator - (Array x) {
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

    ({ from, to, index, iteration_count }) = state;
    value = elements[index];
    index++;
    iteration_count++;

    return ({ ({ from, to, index, iteration_count }), value });
}

int iteratorEnd(mixed state) {
    int from, to, index;

    ({ from, to, index, iteration_count }) = state;

    return is_last_iteration(from, to);
}
