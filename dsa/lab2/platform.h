#include <stdio.h>

#define IS_PLATFORM(x) (defined(PLATFORM_ ## x) && (PLATFORM & PLATFORM_ ## x))

#define PLATFORM_UNSUPPORTED 0
#define PLATFORM_WINDOWS     1 << 0
#define PLATFORM_APPLE       1 << 1
#define PLATFORM_UNIX        1 << 2
#define PLATFORM_HPUX        1 << 3
#define PLATFORM_AIX         1 << 4
#define PLATFORM_SOLARIS     1 << 5

#if defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS // Windows
#elif defined(_WIN64)
    #define PLATFORM PLATFORM_WINDOWS // Windows
#elif defined(__CYGWIN__)
    #define PLATFORM PLATFORM_WINDOWS // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM PLATFORM_APPLE  // Apple OSX and iOS (Darwin)
#elif defined(__ANDROID__)
    #define PLATFORM PLATFORM_UNIX // Android (implies Linux, so it must come first)
#elif defined(__linux__)
    #define PLATFORM PLATFORM_UNIX // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || defined(__MACH__)
    #include <sys/param.h>
    #if defined(BSD)
        #define PLATFORM PLATFORM_UNIX // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
    #else
        #define PLATFORM PLATFORM_UNSUPPORTED
    #endif
#elif defined(__hpux)
    #define PLATFORM PLATFORM_HPUX // HP-UX
#elif defined(_AIX)
    #define PLATFORM PLATFORM_AIX // IBM AIX
#elif defined(__sun) && defined(__SVR4)
    #define PLATFORM PLATFORM_SOLARIS // Oracle Solaris, Open Indiana
#else
    #define PLATFORM PLATFORM_UNSUPPORTED
#endif

/*
For more platforms look into
https://web.archive.org/web/20230222233718/https://opensource.apple.com/source/tidy/tidy-5/tidy/include/platform.h.auto.html
or
https://web.archive.org/web/20230222233629/https://sourceforge.net/p/predef/wiki/OperatingSystems/
or
https://web.archive.org/web/20140625123925/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/
