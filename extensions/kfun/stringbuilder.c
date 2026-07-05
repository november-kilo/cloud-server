#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringbuilder.h"

StringBuilder *sb_create(size_t initial_size) {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    if (!sb) return NULL;

    sb->buffer = malloc(initial_size);
    if (!sb->buffer) {
        free(sb);
        return NULL;
    }

    sb->capacity = initial_size;
    sb->size = 0;
    sb->buffer[0] = '\0';
    return sb;
}

void sb_append(StringBuilder *sb, const char *format, ...) {
    va_list args;
    va_start(args, format);

    size_t remaining = sb->capacity - sb->size;
    int needed = vsnprintf(sb->buffer + sb->size, remaining, format, args);

    if (needed >= remaining) {
        size_t new_capacity = sb->capacity * 2;
        while (sb->size + needed >= new_capacity) {
            new_capacity *= 2;
        }

        char *new_buffer = realloc(sb->buffer, new_capacity);
        if (!new_buffer) {
            va_end(args);
            return;
        }

        sb->buffer = new_buffer;
        sb->capacity = new_capacity;

        va_end(args);
        va_start(args, format);
        vsnprintf(sb->buffer + sb->size, sb->capacity - sb->size, format, args);
    }

    sb->size += needed;
    va_end(args);
}

char *sb_to_string(StringBuilder *sb) {
    char *result = strdup(sb->buffer);
    return result;
}

void sb_free(StringBuilder *sb) {
    if (sb) {
        free(sb->buffer);
        free(sb);
    }
}
