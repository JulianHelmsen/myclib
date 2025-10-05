
.PHONY all: libmylibc.a

OBJECTS = build/sview.o build/arena.o build/minheap.o build/mview.o build/hashmap.o build/file.o build/queue.o build/flag.o build/string_builder.o

libmylibc.a: $(OBJECTS)
	ar rc $@ $(OBJECTS)

build/%.o: src/%.c include/%.h
	$(CC) -g -c -o $@ $< -Wall -Werror -Wextra -pedantic -std=c99 -Iinclude


.PHONY test: libmylibc.a
	$(MAKE) -C tests all
