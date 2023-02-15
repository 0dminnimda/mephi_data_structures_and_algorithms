typedef int error_t;

#define ON_ERROR(expr, on_error) \
    do {                         \
        error_t error = expr;    \
        if (error != 0) {        \
            on_error;            \
        }                        \
    } while (0);

#define WITH_ERROR(expr) ON_ERROR(expr, return error)
