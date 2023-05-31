#include "string_builder.h"

#include <string.h>

typedef struct StringBuilder {
    char *buffer;
    size_t len;
    size_t cap;
} StringBuilder;

StringBuilder *sb_create(size_t init_cap) {
    StringBuilder *sb = (StringBuilder *)malloc(sizeof(StringBuilder));
    sb->buffer = (char *)calloc(init_cap + 1, sizeof(char));
    sb->len = 0;
    sb->cap = init_cap;
    return sb;
}

void sb_append(StringBuilder *sb, const char *str) {
    size_t str_len = strlen(str);
    if (sb->len + str_len >= sb->cap) {
        sb->cap = (sb->cap + str_len) * 2;
        sb->buffer = (char *)realloc(sb->buffer, sb->cap + 1);
    }
    memcpy(sb->buffer + sb->len, str, str_len);
    sb->len += str_len;
    sb->buffer[sb->len] = '\0';
}

char *sb_to_string(StringBuilder *sb) { return sb->buffer; }

void sb_clear(StringBuilder *sb) {
    sb->len = 0;
    sb->buffer[0] = '\0';
}

void sb_free(StringBuilder *sb) {
    free(sb->buffer);
    free(sb);
}
