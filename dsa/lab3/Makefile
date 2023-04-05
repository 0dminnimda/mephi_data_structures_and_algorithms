.PHONY: table_mem
table_mem:
	clang common/input.c table_mem.c main.c -o main.out -O0 -g -Wno-deprecated-declarations

.PHONY: run_table_mem
run_table_mem:
	valgrind --leak-check=full ./main.out

.PHONY: exec_table_mem
exec_table_mem:
	bash exec.sh ./main.out

.PHONY: test_table_mem
test_table_mem:
	clang test_table_mem.c -o test_table_mem.out -O0 -g -Wno-deprecated-declarations
	valgrind --leak-check=full ./test_table_mem.out
