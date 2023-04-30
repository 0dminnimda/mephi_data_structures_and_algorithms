#include <time.h>

long double current_time() {
#if __STDC_VERSION__ >= 201112L  // C11
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long double)ts.tv_sec + (long double)ts.tv_nsec / 1e+9;
#else
    return clock() / CLOCKS_PER_SEC;
#endif
}
