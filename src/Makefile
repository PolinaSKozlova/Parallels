#all, clean, ant, gauss, winograd
CC = g++ -fopenmp
CC_FLAGS = -Wall -Wextra -Werror -std=c++17 #-pedantic-errors
MFLAGS=-MMD -MP
SANITIZER_FLAGS = -fsanitize=address
SRCS=$(wildcard s21*.cc,*/s21*.cc)
OBJ_FILES=$(patsubst %.cc,%.o,$(SRCS))
REPORT_DIR = report


all: interface

interface: clean
	clear
	$(CC) $(CC_FLAGS) interface/interface.cc gauss/gauss.cc winograd/winograd.cc -o app
	./app


ant: clean
	$(CC) $(CC_FLAGS) interface/interface.cc gauss/gauss.cc winograd/winograd.cc -o app
	./app ant


gauss:
	$(CC) $(CC_FLAGS) interface/interface.cc gauss/gauss.cc winograd/winograd.cc -o app
	./app gauss


winograd:
	$(CC) $(CC_FLAGS) interface/interface.cc gauss/gauss.cc winograd/winograd.cc -o app
	./app winograd


clean:
	rm -rf *.o *.a run *.dSYM *.d app *Result.txt


.PHONY:
	all, clean, ant, gauss, winograd, ant

clang:
	clang-format -i --style=Google **/*.cc **/*.h
	clang-format -n --style=Google **/*.cc **/*.h

cppcheck:
	cppcheck --enable=all --language=c++ --std=c++17 --suppress=missingIncludeSystem gauss/* console/console.h winograd/* aco/* matrix/*
