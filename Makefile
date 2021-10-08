# Copyright 2021 Travis Seevers
# Makefile for Regfast-base packages and tests

help:
	echo "make [help|all|hash|regex_if|mem_man]";

all: 
	echo "WIP";


# Hash unit tests
hash: 
	gcc -o hash_tests.out TESTS/hash_test.c hash.c;

# Regex Intermediate Format and Memory Management tests
regex_if: 
	gcc -o regex_if_mm_tests.out TESTS/regex_if_mm_test.c regex_if.c regex_mm.c;

# Regfast Memory Management tests
mem_man: 
	gcc -o mem_man_tests.out TESTS/mem_man_test.c mem_man.c
