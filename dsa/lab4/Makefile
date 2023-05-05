override CFLAGS += -std=c11 -Wno-deprecated-declarations
ifdef BALANCED
	override CFLAGS += -D BALANCED
endif

.PHONY: task
task:
	clang common/common.c main.c -o main.out -O0 -g $(CFLAGS)

.PHONY: task_fast
task_fast:
	clang common/common.c main.c -o main.out -O3 $(CFLAGS)

.PHONY: task_san
task_san:
	clang common/common.c main.c -o main.out -O0 -g $(CFLAGS) -fsanitize=address,undefined

.PHONY: run_task
run_task:
	valgrind --leak-check=full ./main.out

.PHONY: exec_task
exec_task:
	bash exec.sh ./main.out

.PHONY: extra
extra:
	clang common/common.c program.c -o program.out -O0 -g $(CFLAGS) -fsanitize=address,undefined

.PHONY: exec_extra
exec_extra:
	bash exec.sh ./program.out
