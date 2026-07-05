#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <stdlib.h>

typedef struct StringBuilder {
    char* buffer;
    size_t size;
    size_t capacity;
} StringBuilder;

StringBuilder* sb_create(size_t initial_size);
void sb_append(StringBuilder* sb, const char* format, ...);
char* sb_to_string(StringBuilder* sb);
void sb_free(StringBuilder* sb);

#endif
