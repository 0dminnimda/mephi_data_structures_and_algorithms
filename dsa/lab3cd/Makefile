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

.PHONY: test_mem
test_mem:
	clang test_table_mem.c -o test_table_mem.out -O0 -g -Wno-deprecated-declarations
	valgrind --leak-check=full ./test_table_mem.out

.PHONY: ext
ext:
	clang common/input.c main.c -o main.out -O0 -g -Wno-deprecated-declarations -D USE_FILE=1

.PHONY: ext_san
ext_san:
	clang common/input.c main.c -o main.out -O0 -g -Wno-deprecated-declarations -fsanitize=address,undefined -D USE_FILE=1

.PHONY: run_ext
run_ext:
	valgrind --leak-check=full ./main.out
