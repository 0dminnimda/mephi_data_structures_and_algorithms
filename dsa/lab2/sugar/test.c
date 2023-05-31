#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "sugar.h"
#include "../common/input.h"

Error sub_main() {
    char *a = NEW(a, 4);
    strcpy(a, "123");
    printf("Hello, World! %s\n", a);

    RENEW(a, 11);
    printf("Hello, %s! %s\n", PLATFORM_NAME, a);

    strcpy(a, "123456789");
    printf("Hello, %s! %s\n", PLATFORM_NAME, a);

    DELETE(a);
    a = NULL;

    RENEW(a, 11);
    strcpy(a, "123456789");
    printf("Hello, %zu! %s\n", memory_size(a), a);

    RENEW(a, 0);
    printf("Hello, %zu!\n", memory_size(a));

    NEW(a, 0);
    printf("Hello, %zu!\n", memory_size(a));

    DELETE_INPLACE(a) = NULL;
    printf("Hello, %zu! %p\n", memory_size(a), a);

    NEW_OR_ELSE(a, ULLONG_MAX) {
        printf("Hello, new no mem!\n");
    } else {
        printf("Hello, new yes mem!\n");
    }

    RENEW_OR_ELSE(a, ULLONG_MAX) {
        printf("Hello, renew no mem!\n");
    } else {
        printf("Hello, renew yes mem!\n");
        DELETE(a);
    }

    return OK;
}

Error throw() {
    THROW(MAKE_ERROR("FuckError", "Fuck you!"));
}

int main() {
    Error error;

    TRY(sub_main())
    CATCH_ALL {                 
        FPRINT_VERBOSE_ERROR(stderr, error);
    }

    TRY(throw())
    CATCH_ALL {                 
        FPRINT_VERBOSE_ERROR(stderr, error);
    }
    return 0;
}
