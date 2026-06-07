#include "test.h"
#include "process.h"

void process_creation(void) {
    const char* child_argv[] = {
        "dir",
        NULL
    };
    
    process p = process_create(child_argv, 0);
    const int exit = process_wait(p);
    test(exit == 0);

}

void pipe_test(void) {
    const char* child_argv[] = {
        "dir",
        NULL
    };
    
    process p = process_create(child_argv);
    const int exit = process_wait(p);
    test(exit == 0);

}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);
    process_creation();

    return 0;
}
