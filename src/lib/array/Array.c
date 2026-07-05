#include <Array.h>
#include <type.h>

#define NOT_FOUND ({ -1, nil })

inherit Iterable;
inherit format "/lib/util/format";

private mixed *elements;
private int iteration_count;

static void create(mixed *elements) {
    ::elements = elements;
}

string stringify(void) {
    return format::to_string(elements);
}

mixed *get_array(void) {
    return elements;
}

int size(void) {
    return sizeof(elements);
}

int is_empty(void) {
    return size() == 0;
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

mixed find_first(mixed value, varargs int from, int to) {
    Iterator iterator;
    mixed element;

    if (size() == 0) {
        return NOT_FOUND;
    }

    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);

    while (!iterator->end()) {
        element = get(iterator->next());
        if (element == value) {
            return ({ iterator->current(), element });
        }
    }

    return NOT_FOUND;
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
    mixed* filtered_elements;

    ({ from, to }) = fix_from_to(from, to);
    iterator = iterator(from, to);
    filtered_elements = ({ });

    while (!iterator->end()) {
        element = iterator->next();
        if (filterer->evaluate(element)) {
            filtered_elements += ({ element });
        }
    }
    
    return new Array(filtered_elements);
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

/*
 * NAME:	reduce()
 * DESCRIPTION:	reduce array to single value using reducer function
 * PARAMETERS:	reducer - Function to combine elements
 *		from - starting index (optional)
 *		to - ending index (optional)  
 *		initial_value - starting accumulator value (optional)
 */
mixed reduce(Function reducer, varargs mixed args...) {
    Iterator iterator;
    mixed element, accumulator, initial_value;
    int from, to, has_initial, i;
    
    from = 0;
    to = size() - 1;
    has_initial = 0;
    
    if (sizeof(args) >= 1) {
        if (typeof(args[0]) == T_INT) {
            from = args[0];
            if (sizeof(args) >= 2 && typeof(args[1]) == T_INT) {
                to = args[1];
                if (sizeof(args) >= 3) {
                    initial_value = args[2];
                    has_initial = 1;
                }
            } else if (sizeof(args) >= 2) {
                initial_value = args[1];
                has_initial = 1;
            }
        } else {
            initial_value = args[0];
            has_initial = 1;
        }
    }
    
    if (is_empty()) {
        if (has_initial) {
            return initial_value;
        }
        error("Array.reduce: empty array with no initial value");
    }
    
    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);
    
    if (has_initial) {
        accumulator = initial_value;
    } else {
        accumulator = get(iterator->next());
    }
    
    while (!iterator->end()) {
        element = get(iterator->next());
        accumulator = reducer->evaluate(accumulator, element, is_last_iteration(from, to));
    }
    
    return accumulator;
}

string joining(string joiner, varargs int use_to_string) {
    string *str_arr;
    mixed element;
    int i;

    str_arr = ({});
    for (i = 0; i < size(); i++) {
        element = get(i);
        str_arr += ({
            use_to_string ? format::to_string(element) : ((string) element)
        });
    }

    return implode(str_arr, joiner);
}

/*
 * NAME:	group_by()
 * DESCRIPTION:	group elements by a key
 * PARAMETERS:	key - string field name or Function key extractor
 *		remove_key - only works with string field names
 */
mapping group_by(mixed key, varargs int remove_key, int from, int to) {
    Function reducer;
    
    if (typeof(key) == T_STRING) {
        reducer = new GroupingReducer(key, remove_key);
    } else if (typeof(key) == T_OBJECT && key <- Function) {
        reducer = new GroupingByReducer(key);
    } else {
        error("group_by: key must be string or Function");
    }
    
    return reduce(reducer, from, to, ([ ]));
}

mapping partition(Function predicate, varargs int from, int to) {
    Iterator iterator;
    mixed element, *matches, *non_matches;

    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);
    matches = ({ });
    non_matches = ({ });

    while (!iterator->end()) {
        element = get(iterator->next());
        if (predicate->evaluate(element)) {
            matches += ({ element });
        } else {
            non_matches += ({ element });
        }
    }

    return ([ "matches": matches, "non_matches": non_matches ]);
}

mapping partition_by(Function key_extractor, varargs int from, int to) {
    Iterator iterator;
    mixed element, key;
    mapping partitions;

    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);
    partitions = ([ ]);

    while (!iterator->end()) {
        element = get(iterator->next());
        key = key_extractor->evaluate(element);
        
        if (!partitions[key]) {
            partitions[key] = ({ element });
        } else {
            partitions[key] += ({ element });
        }
    }

    return partitions;
}

mapping partition_by_type(varargs int from, int to) {
    Iterator iterator;
    mixed element;
    int type;
    string type_name;
    mapping partitions;

    ({ from, to }) = fix_from_to(from, to);
    iterator = new IntIterator(from, to);
    partitions = ([ ]);

    while (!iterator->end()) {
        element = get(iterator->next());
        type = typeof(element);
        
        switch (type) {
            case T_INT: type_name = "int"; break;
            case T_FLOAT: type_name = "float"; break;
            case T_STRING: type_name = "string"; break;
            case T_ARRAY: type_name = "array"; break;
            case T_MAPPING: type_name = "mapping"; break;
            case T_OBJECT: type_name = "object"; break;
            case T_NIL: type_name = "nil"; break;
            default: type_name = "unknown"; break;
        }
        
        if (!partitions[type_name]) {
            partitions[type_name] = ({ element });
        } else {
            partitions[type_name] += ({ element });
        }
    }

    return partitions;
}

Array sort(void) {
    new MergeSort()->sort(elements);
    return this_object();
}

Array toSorted(void) {
    mixed *new_elements;
    int i;

    new_elements = allocate(size());
    for (i = 0; i < size(); i++) {
        new_elements[i] = elements[i];
    }

    new MergeSort()->sort(new_elements);
    return new Array(new_elements);
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

int is_subset(Array x) {
    Array a;

    a = this_object() - x;
    return sizeof(a->get_array()) == 0;
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
