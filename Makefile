# Copyright 2021 Travis Seevers
# Makefile for Regfast-base packages and tests

help:
	echo "make [help|clean|all|hash|ifr|struct_mem]";

clean:
	rm *.out

all: 
	echo "WIP";


# Hash unit tests
hash: 
	gcc -o hash_tests.out Tests/hash_test.c Core/hash.c Memory/struct_mem.c;

# Regex Intermediate Format and Memory Management tests
ifr: 
	echo "WIP"

# Regfast Memory Management tests
# structure memory tests
struct_mem: 
	gcc -o struct_mem_tests.out Tests/struct_mem_test.c Memory/struct_mem.c
