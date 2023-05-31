override CFLAGS += -g
CC := $(shell which cc 2> /dev/null)
ifndef CC
    $(info INFO: I did not find 'cc', so using 'clang' instead)
    override CFLAGS += -Wno-deprecated-declarations
    CC := clang
	AR := llvm-ar rc
	LIB := -l
else
	AR := ar rcs
	LIB := -l:
endif

QUEUE := list
ifeq ($(QUEUE),vector)
    $(info INFO: using vector implementation of queue (QUEUE=$(QUEUE)))
	override CFLAGS += -D QUEUE_IMPL=\'V\'
else
    $(info INFO: using list implementation of queue (QUEUE=$(QUEUE)))
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
	$(CC) -c -o sugar/custom_malloc.o sugar/custom_malloc.c $(CFLAGS)
	$(AR) sugar.lib sugar/memo.o sugar/custom_malloc.o

.PHONY: test
test:
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/ d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2 d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/ d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15  d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c//10 d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 /2/10 d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f//9" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 /6/9" | ./main.out
	- echo "2 a/0/20" | ./main.out
	- echo "2 a/1/0" | ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9 g/20/3 h/20/5 i/21/4 j/25/9 i/25/3 k/25/7 l/100/10 m/111/9 n/112/3 o/112/10" | ./main.out
	- echo "1 a/1/2 b/1/2 c/10/2" | ./main.out
	- echo "1 a/2/2 b/1/2" | ./main.out

.PHONY: valgrind_test
valgrind_test:
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/ d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2 d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/ d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15  d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c//10 d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 /2/10 d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f//9" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 /6/9" | valgrind --leak-check=full ./main.out
	- echo "2 a/0/20" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/0" | valgrind --leak-check=full ./main.out
	- echo "2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9 g/20/3 h/20/5 i/21/4 j/25/9 i/25/3 k/25/7 l/100/10 m/111/9 n/112/3 o/112/10" | valgrind --leak-check=full ./main.out
	- echo "1 a/1/2 b/1/2 c/10/2" | valgrind --leak-check=full ./main.out
	- echo "1 a/2/2 b/1/2" | valgrind --leak-check=full ./main.out

.PHONY: sugar_test
sugar_test: common sugar
	$(CC) sugar/test.c -o sugar/test.out -L. $(LIB)common.lib $(LIB)sugar.lib $(CFLAGS)
	./sugar/test.out

.PHONY: valgrind_sugar_test
valgrind_sugar_test: common sugar
	$(CC) sugar/test.c -o sugar/test.out -L. $(LIB)common.lib $(LIB)sugar.lib $(CFLAGS)
	valgrind --leak-check=full ./sugar/test.out
