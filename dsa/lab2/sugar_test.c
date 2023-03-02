#include <stdio.h>
#include <stdlib.h>

#include "sugar/sugar.h"
#include "common/input.h"

Error sub_main() {
    char *a = NEW(a, 4);
    strcpy(a, "123");
    printf("Hello, World! %s\n", a);

    RENEW(a, 11);
    strcpy(a, "123456789");
    printf("Hello, %s! %s\n", PLATFORM_NAME, a);

    DELETE(a);
    a = NULL;

    RENEW(a, 11);
    strcpy(a, "123456789");
    printf("Hello, %zu! %s\n", memory_size(a), a);

    DELETE_INPLACE(a) = NULL;
    printf("Hello, %zu! %p\n", memory_size(a), a);

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
