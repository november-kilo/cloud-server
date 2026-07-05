/*
 * KeyExtractor - extract a value from an element by key/field
 */
#include <Function.h>

inherit Function;

private string key_field;
private mixed default_value;

static void create(string field, varargs mixed default_val) {
    key_field = field;
    default_value = default_val;
}

mixed evaluate(varargs mixed args...) {
    mixed element;
    
    element = args[0];
    
    if (typeof(element) == T_MAPPING) {
        if (element[key_field]) {
            return element[key_field];
        }
        return default_value;
    }
    
    if (typeof(element) == T_OBJECT) {
        if (function_object("query_" + key_field, element)) {
            return call_other(element, "query_" + key_field);
        }
        
        if (function_object("get_" + key_field, element)) {
            return call_other(element, "get_" + key_field);
        }
        
        if (function_object(key_field, element)) {
            return call_other(element, key_field);
        }
        return default_value;
    }
    
    return default_value;
}
