#include "file.h"
#include <stdlib.h>


bool file_read_all(FILE* f, file_content* content) {
    fseek(f, 0l, SEEK_END);
    const long size = ftell(f);
    rewind(f);

    if(content->data != NULL) {
        free(content->data);
        content->size = 0;
    }

    content->data = malloc(size);
    if(content->data == NULL) return false;
    content->size = size;


    size_t read = fread(content->data, 1, content->size, f);
    if(read != content->size) {
        if(feof(f)) {
            content->size = read;
            return true;
        }
        if(ferror(f)) {
            free(content->data);
            content->size = 0;
            return false;
        }
    }

    return true;
}

bool file_read_all_at(const char* path, file_content* content) {
    FILE* f = fopen(path, "rb");
    if(f == NULL)
        return false;

    const bool succ = file_read_all(f, content);
    fclose(f);

    return succ;
}
