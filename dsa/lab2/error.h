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
    fprintf(stream, "%s: %s\n", error.type, error.message)

#define STRING_OR_EMPTY(str) ((str == NULL) ? "" : str)
#define MAKE_ERROR_TYPE(type) ((char *)type)
#define MAKE_ERROR(type, message) ((Error){type, STRING_OR_EMPTY(message)})

// Errors
#define NOT_AN_ERROR MAKE_ERROR(NULL, NULL)
#define OK NOT_AN_ERROR

#define VALUE_ERROR_TYPE MAKE_ERROR_TYPE("ValueError")
#define VALUE_ERROR(message) MAKE_ERROR(VALUE_ERROR_TYPE, message)

// Error handling
#define THROW(error) return error;

#define UNIQUE(name) m__unique__variable__name__0123456789__##name##__

#define BASE_TRY(expr) for (Error error = (expr); IS_ERROR(error); error = OK)

#define NO_THROW_TRY(expr)                                 \
    BASE_TRY(expr)                                         \
    for (int UNIQUE(error_i); IS_ERROR(error); error = OK) \
        if (0) 0;

#define TRY(expr)                                        \
    BASE_TRY(expr)                                       \
    for (int UNIQUE(error_i) = 1; UNIQUE(error_i) != 2;) \
        if (UNIQUE(error_i)-- == 0) THROW(error);

#define CATCH_IF(condition) else if ((condition) && (UNIQUE(error_i) = 2))
#define CATCH(target_error) CATCH_IF(ARE_EQUAL_ERRORS(error, target_error))
#define CATCH_ALL else
#define CATCH_N_THROW CATCH_ALL THROW(error)

#endif  // ERROR_H_
