#include <Function.h>

inherit Function;

mixed evaluate(varargs mixed args...) {
    mixed person;
    
    person = args[0];
    
    if (typeof(person) != T_MAPPING || !person["city"] || !person["state"]) {
        return nil;
    }
    
    return person["city"] + ", " + person["state"];
}
