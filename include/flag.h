#ifndef MYLIBC_FLAG_H
#define MYLIBC_FLAG_H


#include <stdbool.h>
#include <stdio.h>
#include <sview.h>

int* flag_opt_int(const char* name, int def_value, const char* descr);
bool* flag_opt_bool(const char* name, bool def_value, const char* descr);
sview* flag_opt_string(const char* name, const char* def_value, const char* descr);

bool* flag_exists(char name, const char* description);


bool flag_parse(int argc, const char** argv);
void flag_reset(void);
void flag_help(FILE* out);
size_t flag_argument_count(void);
sview flag_argument_at(size_t idx);


#endif // MYLIBC_FLAG_H
