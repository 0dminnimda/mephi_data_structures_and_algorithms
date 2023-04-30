.PHONY: mem
mem:
	clang common/common.c main.c -o main.out -std=c11 -O0 -g -Wno-deprecated-declarations

.PHONY: mem_fast
mem_fast:
	clang common/common.c main.c -o main.out -std=c11 -O3 -Wno-deprecated-declarations

.PHONY: mem_san
mem_san:
	clang common/common.c main.c -o main.out -std=c11 -O0 -g -Wno-deprecated-declarations -fsanitize=address,undefined

.PHONY: run_mem
run_mem:
	valgrind --leak-check=full ./main.out

.PHONY: exec_mem
exec_mem:
	bash exec.sh ./main.out
