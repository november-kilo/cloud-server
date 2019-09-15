#include <Json.h>

mapping parse(string json) {
    return JSON_PARSER->parse(json);
}
