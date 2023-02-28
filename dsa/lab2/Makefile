override CFLAGS += -g
CC := $(shell which cc 2> /dev/null)
ifndef CC
    $(info INFO: I did not find 'cc', so using 'clang' instead)
    override CFLAGS += -Wno-deprecated-declarations
    CC := clang
	AR := llvm-ar rc
	LIB_PREFIX := 
	LIB_EXT := .lib
else
	# clang uses it automatically + does not support it for every target
    override CFLAGS += -fPIC
	AR := ar rcs
	LIB_PREFIX := lib
	LIB_EXT := .a
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
	$(CC) passenger.c load_balancer.c main.c -o main.out -L. -lcommon -lqueue -lsugar $(CFLAGS)

.PHONY: queue
queue:
	$(CC) -c -o queue/queue.o queue/queue.c $(CFLAGS)
	$(AR) $(LIB_PREFIX)queue$(LIB_EXT) queue/queue.o

.PHONY: common
common:
	$(CC) -c -o common/input.o common/input.c $(CFLAGS)
	$(CC) -c -o common/string.o common/string.c $(CFLAGS)
	$(AR) $(LIB_PREFIX)common$(LIB_EXT) common/input.o common/string.o

.PHONY: sugar
sugar:
	$(CC) -c -o sugar/memo.o sugar/memo.c $(CFLAGS)
	$(AR) $(LIB_PREFIX)sugar$(LIB_EXT) sugar/memo.o

.PHONY: test
test: common sugar
	$(CC) sugar_test.c -o test.out -lcommon -lsugar $(CFLAGS)
	./test.out
