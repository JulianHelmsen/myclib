#ifndef MYLIBC_FILE_H
#define MYLIBC_FILE_H

#include <stdio.h>
#include <stdbool.h>


typedef struct {
    size_t size;
    void* data;
} file_content;



bool file_read_all(FILE* f, file_content* content);
bool file_read_all_at(const char* path, file_content* content);

#endif  //MYLIBC_FILE_H
