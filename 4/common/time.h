long double current_time();

#define TIMEIT(timing, code)                \
    {                                       \
        long double begin = current_time(); \
        code;                               \
        timing += current_time() - begin;   \
    }
