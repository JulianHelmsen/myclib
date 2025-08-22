
.PHONY all: libmylibc.a

OBJECTS = build/sview.o build/arena.o

libmylibc.a: $(OBJECTS)
	ar rc $@ $(OBJECTS)

build/%.o: src/%.c include/%.h
	$(CC) -c -o $@ $< -Wall -Werror -Wextra -pedantic -std=c99 -Iinclude


.PHONY tests: libmylibc.a
	$(MAKE) -C test all
