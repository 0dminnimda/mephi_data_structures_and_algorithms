#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *type;
    char *message;
} Error;

#define COMPARE_ERRORS(error1, error2) \
    ((error1.type == error2.type) ? 0 : strcmp(error1.type, error2.type))
#define ARE_EQUAL_ERRORS(error1, error2) (COMPARE_ERRORS(error1, error2) == 0)

#define IS_ERROR(error) (error.type != NULL)
#define FPRINT_ERROR(stream, error) \
    fprintf(stream, "%s: %s", error.type, error.message)

#define STRING_OR_EMPTY(str) ((str == NULL) ? "" : str)
#define MAKE_ERROR_TYPE(type) ((char *)type)
#define MAKE_ERROR(type, message) ((Error){type, STRING_OR_EMPTY(message)})

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

#endif  // ERROR_H_
