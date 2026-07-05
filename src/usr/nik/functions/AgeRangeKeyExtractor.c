#include <Function.h>

inherit Function;

mixed evaluate(varargs mixed args...) {
    mixed person;
    int age;
    
    person = args[0];
    
    if (typeof(person) != T_MAPPING || !person["age"]) {
        return nil;
    }
    
    age = person["age"];
    
    if (age < 18) return "minor";
    if (age < 30) return "young_adult";
    if (age < 50) return "adult";
    if (age < 65) return "middle_aged";
    return "senior";
}
