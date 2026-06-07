#ifndef PROCESS_H
#define PROCESS_H


typedef struct {
    int pid;
} process;


typedef struct {
    int unused;
} process_creation_args ;


process process_create_impl(const char** argv, process_creation_args args);

#define process_create(argv, ...) process_create_impl(argv, (process_creation_args) {__VA_ARGS__})
int process_wait(process p);

#endif // PROCESS_H
