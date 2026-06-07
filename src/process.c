#include "process.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "macros.h"

#ifdef __unix__
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#endif


process process_create_impl(const char** argv, process_creation_args args) {
#ifdef __unix__
    UNUSED(args);
    pid_t pid = fork();
    if(pid == -1) {
        ERR("fork: %s\n", strerror(errno));
        abort();
    }
    if(pid == 0) {
        // child
        execvp(argv[0], (char**) argv);
        perror("execvp");
        abort();
    }

    return (process) {
        .pid = pid,
    };
#else
#error "process_create not implemented for this platform"
#endif

}

int process_wait(process p) {
#ifdef __unix__
    int status;
    int ret = 0;
    do {
        ret = waitpid(p.pid, &status, 0);
        if(ret == -1 && errno != EINTR) {
            ERR("waitpid: %s\n", strerror(errno));
            abort();
        }
    }while(ret == -1 && errno == EINTR);

    if(WIFEXITED(status)) {
        return WEXITSTATUS(status);
        
    }
    return -1;
#else
#error "process_wait not implemented for this platform"
#endif

}

