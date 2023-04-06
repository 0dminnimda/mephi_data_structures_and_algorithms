.PHONY: mem
mem:
	clang common/input.c table.c item_mem.c main.c -o main.out -O0 -g -Wno-deprecated-declarations

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
