/*
 * GroupingByReducer - group elements using a custom key extractor function
 * More flexible but doesn't support key removal
 */
#include <Function.h>

inherit Function;

private Function key_extractor;

static void create(Function extractor) {
    key_extractor = extractor;
}

mixed evaluate(varargs mixed args...) {
    mixed accumulator, element, key_value;
    mapping groups;
    
    accumulator = args[0];
    element = args[1];
    
    if (typeof(accumulator) != T_MAPPING) {
        groups = ([ ]);
    } else {
        groups = accumulator;
    }
    
    key_value = key_extractor->evaluate(element);
    
    if (key_value == nil) {
        return groups;
    }
    
    if (!groups[key_value]) {
        groups[key_value] = ({ element });
    } else {
        groups[key_value] += ({ element });
    }
    
    return groups;
}
