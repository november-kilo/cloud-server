#include <type.h>

string from_map(mixed value, varargs mapping seen) {
    string str;
    int i, sz;
    mixed *indices, *values;

    if (seen == nil) {
        seen = ([]);
    }

    switch (typeof(value)) {
    case T_FLOAT:
	str = (string) value;
	if (sscanf(str, "%*s.") == 0 && sscanf(str, "%*se") == 0) {
	    str += ".0";
	}
	return str;

    case T_INT:
	return (string) value;

    case T_STRING:
	str = value;
	if (sscanf(str, "%*s\\") != 0) {
	    str = implode(explode("\\" + str + "\\", "\\"), "\\\\");
	}
	if (sscanf(str, "%*s\"") != 0) {
	    str = implode(explode("\"" + str + "\"", "\""), "\\\"");
	}
	if (sscanf(str, "%*s\n") != 0) {
	    str = implode(explode("\n" + str + "\n", "\n"), "\\n");
	}
	if (sscanf(str, "%*s\t") != 0) {
	    str = implode(explode("\t" + str + "\t", "\t"), "\\t");
	}
	return "\"" + str + "\"";

    case T_OBJECT:
	str = "";
	catch(str = value->to_string());
	return from_map("<" + object_name(value) + (str == "" || str == nil ? "" : ": " + str + " ") + ">", seen);

    case T_ARRAY:
	if (seen[value]) {
	    return "" + (seen[value] - 1);
	}

	seen[value] = map_sizeof(seen) + 1;
	sz = sizeof(value);
	if (sz == 0) {
	    return "[ ]";
	}

	str = "[ ";
	for (i = 0, --sz; i < sz; i++) {
	    str += from_map(value[i], seen) + ", ";
	}
	return str + from_map(value[i], seen) + " ]";

    case T_MAPPING:
	if (seen[value]) {
	    return "" + (seen[value] - 1);
	}

	seen[value] = map_sizeof(seen) + 1;
	sz = map_sizeof(value);
	if (sz == 0) {
	    return "{ }";
	}

	str = "{ ";
	indices = map_indices(value);
	values = map_values(value);
	for (i = 0, --sz; i < sz; i++) {
	    str += from_map("" + indices[i], seen) + ":" + from_map(values[i], seen) + ", ";
	}
	return str + from_map("" + indices[i], seen) + ":" + from_map(values[i], seen) + " }";

    case T_NIL:
	return "null";
    }
}

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

static mapping *makeKeyValuePair(mixed *kv) {
    return ({ ([kv[0] : kv[2]]) });
}

static mixed *handleEmptyJsonArray(mixed *jsonArray) {
    return ({ ({}) });
}

static mixed *handleJsonArray(mixed *jsonArray) {
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

static mapping *handleJsonObject(mixed *jsonObj) {
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

static string *handleString(mixed *value) {
    return ({ implode(explode(value[0], "\""), "") });
}

static int *handleInteger(mixed *value) {
    int i;
    return sscanf(value[0], "%d", i) == 1 ? ({ i }) : ({});
}

static float *handleFloat(mixed *value) {
    float f;
    return sscanf(implode(value, ""), "%f", f) == 1 ? ({ f }) : ({});
}

mapping to_map(string json) {
    mixed *parsed;
    mapping map;
    parsed = parse_string(JSON_PARSER_GRAMMAR, json);
    map = ([]);
    map["json"] = parsed[0];
    return map;
}
