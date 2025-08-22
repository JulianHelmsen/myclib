TESTS = sview


all: $(TESTS)

%: %.c ../libmylibc.a test.h
	$(CC) -o $@ $< -L.. -lmylibc -Wall -Werror -pedantic -Wextra -std=c99 -I../include
	./$@ || 1


