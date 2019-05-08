#include <type.h>

#define JSON_PARSER_GRAMMAR "whitespace = /[\n ]*/ " + \
        "colon = /:/ " + \
        "comma = /,/ " + \
        "string = /\"([^\\\\\"\n]*(\\\\.)*)*\"/ " + \
        "float = /[0-9]+\\.[0-9]+([eE][-+]?[0-9]+)?/ " + \
        "float = /\\.[0-9]+([eE][-+]?[0-9]+)?/ " + \
        "float = /[0-9]+[eE][-+]?[0-9]+/ " + \
        "integer = /[-+]?[0-9]+/ " + \
        "special = /(true|false|null)/ " + \
        "S : jsonValue " + \
        "jsonObject : '{' jsonObjectPairs '}' ? handleJsonObject " + \
        "jsonObjectPairs : jsonObjectPair " + \
        "jsonObjectPairs : jsonObjectPairs comma jsonObjectPair " + \
        "jsonObjectPair : jsonKey colon jsonValue ? makeKeyValuePair " + \
        "jsonArray : '[' jsonArrayElements ']' ? handleJsonArray " + \
        "jsonArray : '[]' ? handleEmptyJsonArray " + \
        "jsonArrayElements : jsonObject " + \
        "jsonArrayElements : jsonValue " + \
        "jsonArrayElements : jsonArrayElements comma jsonValue " + \
        "jsonKey : string ? handleString " + \
        "jsonValue: jsonObject " + \
        "jsonValue: jsonArray " + \
        "jsonValue: string ? handleString " + \
        "jsonValue: number " + \
        "jsonValue: special ? handleString " + \
        "number: integer ? handleInteger " + \
        "number: float ? handleFloat "

mapping *makeKeyValuePair(mixed *kv) {
    return ({ ([kv[0] : kv[2]]) });
}

mixed *handleEmptyJsonArray(mixed *jsonArray) {
    return ({ ({}) });
}

mixed *handleJsonArray(mixed *jsonArray) {
    int i;
    int j;
    int sz;
    mixed *jsonResults;

    sz = sizeof(jsonArray);
    jsonResults = ({});
    for (i = 1, j = 0; i < sz; i += 2, j++) {
        jsonResults += ({ jsonArray[i] });
    }
    return ({ jsonResults });
}

mapping *handleJsonObject(mixed *jsonObj) {
    int i;
    int sz;
    mapping jsonResults;

    jsonResults = ([]);
    for (i = 0, sz = sizeof(jsonObj); i < sz; i++) {
        if (typeof(jsonObj[i]) == T_MAPPING) {
            jsonResults += jsonObj[i];
        }
    }
    return ({ jsonResults });
}

string *handleString(mixed *value) {
    return ({ implode(explode(value[0], "\""), "") });
}

int *handleInteger(mixed *value) {
    int i;
    return sscanf(value[0], "%d", i) == 1 ? ({ i }) : ({});
}

float *handleFloat(mixed *value) {
    float f;
    return sscanf(implode(value, ""), "%f", f) == 1 ? ({ f }) : ({});
}

mapping parse(string json) {
    mixed *parsed;
    mapping map;
    parsed = parse_string(JSON_PARSER_GRAMMAR, json);
    map = ([]);
    map["json"] = parsed[0];
    return map;
}
