override CFLAGS += -g
CC := $(shell which cc 2> /dev/null)
ifndef CC
    $(info INFO: I did not find 'cc', so using 'clang' instead)
    override CFLAGS += -Wno-deprecated-declarations
    CC := clang
endif

QUEUE := list
ifeq ($(QUEUE),vector)
    $(info INFO: using vector implementation of queue)
	override CFLAGS += -D QUEUE_IMPL='V'
else
    $(info INFO: using list implementation of queue)
endif

.PHONY: all
all: build

.PHONY: build
build:
	$(CC) common/input.c common/string.c sugar/memo.c passenger.c load_balancer.c queue/queue.c main.c -o main.out $(CFLAGS)

.PHONY: test
test:
	$(CC) common/input.c sugar/memo.c sugar_test.c -o test.out $(CFLAGS)
	./test.out
