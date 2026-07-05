/*
 * GroupingReducer - group mappings by a specific field name
 * Supports removing the grouping field from elements
 */
#include <Function.h>

inherit Function;
inherit mapping_util "/lib/util/mapping";

private string key_field;
private int remove_key;

static void create(string key_field, varargs int remove_key) {
    ::key_field = key_field;
    ::remove_key = !!remove_key;
}

mixed evaluate(varargs mixed args...) {
    mixed accumulator, element, key_value, cleaned_element;
    mapping groups;
    
    accumulator = args[0];
    element = args[1];
    
    if (typeof(accumulator) != T_MAPPING) {
        groups = ([ ]);
    } else {
        groups = accumulator;
    }
    
    if (typeof(element) != T_MAPPING || !element[key_field]) {
        return groups;
    }
    
    key_value = element[key_field];
    
    if (remove_key) {
        cleaned_element = map_remove(element, key_field);
    } else {
        cleaned_element = element;
    }
    
    if (!groups[key_value]) {
        groups[key_value] = ({ cleaned_element });
    } else {
        groups[key_value] += ({ cleaned_element });
    }
    
    return groups;
}
