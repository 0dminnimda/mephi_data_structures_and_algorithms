.PHONY: mem
mem:
	clang common/input.c main.c -o main.out -O0 -g -Wno-deprecated-declarations

.PHONY: mem_san
mem_san:
	clang common/input.c main.c -o main.out -O0 -g -Wno-deprecated-declarations -fsanitize=address,undefined

.PHONY: run_mem
run_mem:
	valgrind --leak-check=full ./main.out

.PHONY: exec_mem
exec_mem:
	bash exec.sh ./main.out
