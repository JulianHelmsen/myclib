#ifndef MYLIBC_MACROS_H
#define MYLIBC_MACROS_H

#include <stdlib.h>
#include <stdio.h>

#if defined(NDEBUG)
#define LOGGING_ENABLE 0
#else
#define LOGGING_ENABLE 1
#endif

#ifndef LOG
#if LOGGING_ENABLE
#define LOG(...) printf("[LOG]: " __VA_ARGS__)
#else
#define LOG(...)
#endif // LOGGING_ENABLE
#endif // LOG
    

#ifndef ERR
#define ERR_STR_HELPER(x) #x
#define ERR_STR(x) ERR_STR_HELPER(x)
#define ERR(...) fprintf(stderr, "[ERROR at "__FILE__ ":"ERR_STR(__LINE__)"]: " __VA_ARGS__)
#endif // ERR



#ifndef UNUSED
#define UNUSED(var) do {(void) (var); } while(0)
#endif // UNUSED


#ifndef UNREACHABLE
#define UNREACHABLE() do {\
        fflush(stdout); \
        ERR("UNREACHABLE()\n"); \
        abort(); \
    } while(0)
#endif //UNREACHABLE


#endif
