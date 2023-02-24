#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "input.h"
#include "memo.h"
#include "platform.h"

Error sub_main() {
    char *a = NEW(a, 4);
    strcpy(a, "123");
    printf("Hello, World! %s\n", a);

    RENEW(a, 11);
    strcpy(a, "123456789");
    printf("Hello, %s! %s\n", PLATFORM_NAME, a);

    DELETE(a);

    return OK;
}

int main() {
    NO_THROW_TRY(sub_main())
    CATCH_ALL {                 
        FPRINT_ERROR(stderr, error);
    }
    return 0;
}
