# Copyright 2021 Travis Seevers
# Makefile for Regfast-base packages and tests

help:
	echo "make [help|all|hash|regex_if|mem_man]";

all: 
	echo "WIP";


# Hash unit tests
hash: 
	gcc -o hash_tests.out Tests/hash_test.c Core/hash.c Memory/mem_man.c;

# Regex Intermediate Format and Memory Management tests
regex_if: 
	gcc -o regex_if_mm_tests.out Tests/regex_if_mm_test.c Core/regex_if.c Memory/mem_man.c;

# Regfast Memory Management tests
mem_man: 
	gcc -o mem_man_tests.out Tests/mem_man_test.c Memory/mem_man.c
