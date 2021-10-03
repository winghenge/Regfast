# Copyright 2021 Travis Seevers
# Makefile for Regfast-base packages and tests

all:
	echo "WIP"


# Hash unit tests
hash:
	gcc -o hash_tests.out TESTS/hash_test.c hash.c
