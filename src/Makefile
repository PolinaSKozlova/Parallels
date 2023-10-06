#all, clean, ant, gauss, winograd, test
CC = g++
CC_FLAGS = -Wall -Wextra -Werror -std=c++17 #-pedantic-errors
MFLAGS=-MMD -MP
TEST_FLAGS = -lgtest -pthread
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage 
SANITIZER_FLAGS = -fsanitize=address
SRCS=$(wildcard s21*.cc,*/s21*.cc)
OBJ_FILES=$(patsubst %.cc,%.o,$(SRCS))
REPORT_DIR = report


interface: clean
	clear
	$(CC) $(CC_FLAGS) interface/interface.cc gauss/gauss.cc winograd/winograd.cc -o app -g
	./app


all: interface

gauss:
	$(CC) $(CC_FLAGS) $(MFLAGS) gauss/*.cc  -o gauss
	rm -rf gauss.dSYM & clear
	./gauss

clean:
	rm -rf *.o *.gcno *.gcda *.info *.a $(REPORT_DIR) g_tests run *.dSYM *.d app

test: 
	clear
	$(CC) $(CC_FLAGS) $(SANITIZER_FLAGS) $(MFLAGS) gauss/*.cc gauss_tests/gauss_*.cc -o g_tests $(TEST_FLAGS) 
	./g_tests

gcov_report:
	clear
	$(CC) $(CC_FLAGS) $(MFLAGS) $(COVERAGE_FLAGS) gauss/*.cc gauss_tests/gauss_*.cc -o g_tests $(TEST_FLAGS)
	./g_tests
	lcov -t "$@" -o $@.info -c -d . --no-external  --ignore-errors mismatch --quiet
	lcov -r $@.info "*include*" -o $@.info  --ignore-errors unused --quiet
	genhtml -o ./$(REPORT_DIR) $@.info
	open ./report/index.html

# %.o:%.cc
# 	$(CC) -c $(CC_FLAGS) $< -o $@ -I .

valgrind: test
	valgrind --leak-check=full \
        	 --show-leak-kinds=all \
         	 --track-origins=yes \
         	 --verbose \
			 ./g_tests

leaks:  test
	CK_FORK=no leaks -atExit -- ./g_tests

.PHONY:
	all, clean, test, gcov_report, ant, gauss, winograd, ant

clang:
	clang-format -i --style=Google gauss/*.cc gauss/*.h
	clang-format -n --style=Google gauss/*.cc gauss/*.h

cppcheck:
	cppcheck --enable=all --language=c++ --std=c++17 --suppress=missingIncludeSystem gauss/* console/console.h winograd/*

run: clean
	clear
	$(CC) $(CC_FLAGS) gauss/*.cc  winograd/*.cc run.cc -o run -g
	./run

main: clean
	clear
	$(CC) $(CC_FLAGS) winograd/winograd2.cc main.cc -o run	
	./run

ant: clean
	clear
	$(CC) $(CC_FLAGS) aco/aco.cc ant.cc -o run	
	./run

push:
	git push origin predevelop
