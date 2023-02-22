#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    char *message;
} Error;

#define COMPARE_ERRORS(error1, error2) \
    ((error1.name == error2.name) ? 0 : strcmp(error1.name, error2.name))
#define ARE_EQUAL_ERRORS(error1, error2) (COMPARE_ERRORS(error1, error2) == 0)

#define IS_ERROR(error) (error.name != NULL)
#define STRING_OR_EMPTY(str) ((str == NULL) ? "" : str)
#define MAKE_ERROR(name, message) ((Error){name, STRING_OR_EMPTY(message)})
#define FPRINT_ERROR(stream, error) \
    fprintf(stream, "%s: %s", error.name, error.message)

// Errors
#define NOT_AN_ERROR MAKE_ERROR(NULL, NULL)
#define OK NOT_AN_ERROR

#define ON_ERROR(expr, on_error) \
    do {                         \
        Error error = expr;      \
        if (IS_ERROR(error)) {   \
            on_error;            \
        }                        \
    } while (0);

#define WITH_ERROR(expr) ON_ERROR(expr, return error)

#define TRY(expr) WITH_ERROR(expr)
#define THROW_ERRORS(expr)           \
    ON_ERROR(expr, {                 \
        FPRINT_ERROR(stderr, error); \
        return -1;                   \
    })

#define CALL(func, ...) WITH_ERROR(func(__VA_ARGS__))
