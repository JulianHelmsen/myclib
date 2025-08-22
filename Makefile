
.PHONY all: libmylibc.a

OBJECTS = build/sview.o build/arena.o build/minheap.o

libmylibc.a: $(OBJECTS)
	ar rc $@ $(OBJECTS)

build/%.o: src/%.c include/%.h
	$(CC) -c -o $@ $< -Wall -Werror -Wextra -pedantic -std=c99 -Iinclude


.PHONY test: libmylibc.a
	$(MAKE) -C tests all
