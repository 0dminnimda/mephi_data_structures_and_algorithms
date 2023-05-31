#include <stdlib.h>

typedef struct StringBuilder StringBuilder;

StringBuilder *sb_create(size_t init_cap);
void sb_append(StringBuilder *sb, const char *str);
char *sb_to_string(StringBuilder *sb);
void sb_clear(StringBuilder *sb);
void sb_free(StringBuilder *sb);
