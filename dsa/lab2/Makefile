override CFLAGS += -g
CC := $(shell which cc 2> /dev/null)
ifndef CC
    $(info INFO: I did not find 'cc', so using 'clang' instead)
    override CFLAGS += -Wno-deprecated-declarations
    CC := clang
	AR := llvm-ar rc
	LIB := -l
else
	# clang uses it automatically + does not support it for every target
    override CFLAGS += -fPIC
	AR := ar rcs
	LIB := -l:
endif

QUEUE := list
ifeq ($(QUEUE),vector)
    $(info INFO: using vector implementation of queue)
	override CFLAGS += -D QUEUE_IMPL='V'
else
    $(info INFO: using list implementation of queue)
endif
$(info )

.PHONY: all
all: build

.PHONY: build
build: common queue sugar
	$(CC) passenger.c load_balancer.c main.c -o main.out \
		-L. $(LIB)common.lib $(LIB)queue.lib $(LIB)sugar.lib $(CFLAGS)

.PHONY: queue
queue:
	$(CC) -c -o queue/queue.o queue/queue.c $(CFLAGS)
	$(AR) queue.lib queue/queue.o

.PHONY: common
common:
	$(CC) -c -o common/input.o common/input.c $(CFLAGS)
	$(CC) -c -o common/string.o common/string.c $(CFLAGS)
	$(AR) common.lib common/input.o common/string.o

.PHONY: sugar
sugar:
	$(CC) -c -o sugar/memo.o sugar/memo.c $(CFLAGS)
	$(AR) sugar.lib sugar/memo.o

.PHONY: test
test: common sugar
	$(CC) sugar_test.c -o test.out -L. $(LIB)common.lib $(LIB)sugar.lib $(CFLAGS)
	./test.out
