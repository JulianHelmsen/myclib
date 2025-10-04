TESTS = sview arena da minheap mview hashmap file queue
TEST_FILES = $(addsuffix .test, $(TESTS))

all: $(TEST_FILES)

%.test: %.c ../libmylibc.a test.h
	$(CC) -g -o $@ $< -L.. -lmylibc -Wall -Werror -pedantic -Wextra -std=c99 -I../include
	./$@ || 1


