#define _POSIX_C_SOURCE 199309L

#include <time.h>

long double current_time() {
#if (_POSIX_TIMERS > 0) && (_POSIX_C_SOURCE >= 199309L)
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) return -1;
    return (long double)ts.tv_sec + (long double)ts.tv_nsec / 1e+9;
#elif __STDC_VERSION__ >= 201112L  // C11
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long double)ts.tv_sec + (long double)ts.tv_nsec / 1e+9;
#else
    return clock() / CLOCKS_PER_SEC;
#endif
}
