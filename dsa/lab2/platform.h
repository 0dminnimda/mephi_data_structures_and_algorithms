#include <stdio.h>

#define IS_PLATFORM(x) ((PLATFORM_ ## x) && ((PLATFORM) & (PLATFORM_ ## x)))
#define IS_UNSUPPORTED_PLATFORM (PLATFORM == UNSUPPORTED_PLATFORM)

#define UNSUPPORTED_PLATFORM 0

#define PLATFORM_WINDOWS     1 << 0
#define PLATFORM_WINDOWS_32  1 << 1 | PLATFORM_WINDOWS
#define PLATFORM_WINDOWS_64  1 << 2 | PLATFORM_WINDOWS
#define PLATFORM_CYGWIN      1 << 3 | PLATFORM_WINDOWS

#define PLATFORM_UNIX        1 << 10
#define PLATFORM_LINUX       1 << 11 | PLATFORM_UNIX
#define PLATFORM_BSD         1 << 12 | PLATFORM_UNIX
#define PLATFORM_HPUX        1 << 13 | PLATFORM_UNIX
#define PLATFORM_AIX         1 << 14 | PLATFORM_UNIX
#define PLATFORM_SOLARIS     1 << 15 | PLATFORM_UNIX

#define PLATFORM_APPLE       1 << 20 | PLATFORM_BSD

#if defined(_WIN64)
    // Windows 64-bit
    #define PLATFORM_NAME "Windows (64-bit)"
    #define PLATFORM PLATFORM_WINDOWS_64
#elif defined(_WIN32)
    // Windows 32-bit
    #define PLATFORM_NAME "Windows (32-bit)"
    #define PLATFORM PLATFORM_WINDOWS_32
#elif defined(__CYGWIN__)
    // Windows (Cygwin POSIX under Microsoft Window)
    #define PLATFORM_NAME "Cygwin"
    #define PLATFORM PLATFORM_CYGWIN
#elif defined(__APPLE__) && defined(__MACH__)
    // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #define PLATFORM_NAME "iOS" // Apple iOS
    #elif TARGET_OS_IPHONE == 1
        #define PLATFORM_NAME "iOS" // Apple iOS
    #elif TARGET_OS_MAC == 1
        #define PLATFORM_NAME "OSX" // Apple OSX
    #else
        #define PLATFORM_NAME "Apple Platform"
    #endif
    #define PLATFORM PLATFORM_APPLE
#elif defined(__APPLE__)
    #define PLATFORM_NAME "MacOS (Classic)"
    #define PLATFORM PLATFORM_APPLE
#elif defined(__ANDROID__)
    // Android (implies Linux, so it must come first)
    #define PLATFORM_NAME "Android"
    #define PLATFORM PLATFORM_LINUX
#elif defined(__linux__)
    // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
    #define PLATFORM_NAME "Linux"
    #define PLATFORM PLATFORM_LINUX
#elif defined(__unix__)
    #include <sys/param.h>
    #if defined(BSD)
        // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
        #define PLATFORM_NAME "BSD"
        #define PLATFORM PLATFORM_BSD
    #else
        #define PLATFORM_NAME "Unix Platform"
        #define PLATFORM PLATFORM_UNIX
    #endif
#elif defined(__hpux)
    // HP-UX
    #define PLATFORM_NAME "HP-UX"
    #define PLATFORM PLATFORM_HPUX
#elif defined(_AIX)
    // IBM AIX
    #define PLATFORM_NAME "AIX"
    #define PLATFORM PLATFORM_AIX
#elif defined(__sun) && defined(__SVR4)
    // Oracle Solaris, Open Indiana
    #define PLATFORM_NAME "Solaris"
    #define PLATFORM PLATFORM_SOLARIS
#else
    #define PLATFORM_NAME "Unsupported Platform"
    #define PLATFORM UNSUPPORTED_PLATFORM
#endif


/*
For more platforms look into
https://web.archive.org/web/20230222233718/https://opensource.apple.com/source/tidy/tidy-5/tidy/include/platform.h.auto.html
or
https://web.archive.org/web/20230222233629/https://sourceforge.net/p/predef/wiki/OperatingSystems/
or
https://web.archive.org/web/20140625123925/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/
