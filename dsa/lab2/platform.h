#include <stdio.h>

typedef enum {
    Windows,
    Apple,
    Unix,
    Unsupported
} Platform;

#if defined(_WIN32)
    #define PLATFORM Windows // Windows
#elif defined(_WIN64)
    #define PLATFORM Windows // Windows
#elif defined(__CYGWIN__)
    #define PLATFORM Windows // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__APPLE__) && defined(__MACH__)
    // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #define PLATFORM Apple // Apple iOS
    #elif TARGET_OS_IPHONE == 1
        #define PLATFORM Apple // Apple iOS
    #elif TARGET_OS_MAC == 1
        #define PLATFORM Apple // Apple OSX
    #else
        #define PLATFORM Unsupported
    #endif
#elif defined(__ANDROID__)
    #define PLATFORM Unix // Android (implies Linux, so it must come first)
#elif defined(__linux__)
    #define PLATFORM Unix // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || defined(__MACH__)
    #include <sys/param.h>
    #if defined(BSD)
        #define PLATFORM Unix // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
    #else
        #define PLATFORM Unsupported
    #endif
#elif defined(__hpux)
    #define PLATFORM Unsupported // HP-UX
#elif defined(_AIX)
    #define PLATFORM Unsupported // IBM AIX
#elif defined(__sun) && defined(__SVR4)
    #define PLATFORM Unsupported // Oracle Solaris, Open Indiana
#else
    #define PLATFORM Unsupported
#endif
