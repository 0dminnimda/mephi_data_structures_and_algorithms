#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *ErrorType;

typedef struct {
    ErrorType type;
    char *message;
    char *line;
} Error;

static inline int error_eq(ErrorType type1, ErrorType type2) {
    if (type1 == type2) return 1;
    if (type1 == NULL || type2 == NULL) return 0;
    return strcmp(type1, type2);
}

#define ARE_EQUAL_ERRORS(error_type1, error_type2) \
    error_eq((error_type1), (error_type2))

#define IS_ERROR(error) ((error).type != NULL)

#define STRING_OR_DEFAULT(str, default) (((str) == NULL) ? default : str)
#define STRING_OR_EMPTY(str) STRING_OR_DEFAULT(str, "")
#define FPRINT_ERROR(stream, error)                                       \
    fprintf(stream, "%s: %s\n", STRING_OR_DEFAULT((error).type, "Error"), \
            STRING_OR_EMPTY((error).message))

#define FPRINT_VERBOSE_ERROR(stream, error) \
    printf("\n");                           \
    FPRINT_ERROR(stream, error);            \
    printf("The error occurred at ./%s\n", error.line);

#define STR(x) #x
#define STR_EXPAND(x) STR(x)

#define CURRENT_LINE (__FILE__ ":" STR_EXPAND(__LINE__))

#define MAKE_ERROR_TYPE(type) ((ErrorType)(type))
#define MAKE_ERROR(type, message) ((Error){(type), (message), CURRENT_LINE})

// Errors
#define NOT_AN_ERROR MAKE_ERROR(NULL, NULL)
#define OK NOT_AN_ERROR

#define VALUE_ERROR_TYPE MAKE_ERROR_TYPE("ValueError")
#define VALUE_ERROR(message) MAKE_ERROR(VALUE_ERROR_TYPE, message)

// Error handling
#define THROW(error) \
    { return (error); }

#define NOOP 0

#define TRY(expr)                  \
    if (!IS_ERROR(error = (expr))) \
        ;
#define AUTO_TRY(expr)          \
    {                           \
        Error error;            \
        TRY(expr) CATCH_N_THROW \
    }

#define CATCH(error_type) else if (ARE_EQUAL_ERRORS(error.type, (error_type)))
#define CATCH_ALL else if (1)
#define CATCH_N_THROW CATCH_ALL THROW(error)

#endif  // ERROR_H_
