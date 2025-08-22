#ifndef MYLIBC_DA_H
#define MYLIBC_DA_H

#define DA_DEFAULT_CAPACITY 10

#include <stdlib.h>
#include <stdio.h>

#define da_append(pda, value) \
    do { \
        if((pda)->size + 1 > (pda)->capacity) { \
            (pda)->capacity = (pda)->capacity < DA_DEFAULT_CAPACITY ? DA_DEFAULT_CAPACITY : (pda)->capacity * 2; \
            (pda)->items = realloc((pda)->items, (pda)->capacity * sizeof(*(pda)->items)); \
            if((pda)->items == NULL) { \
                perror("da_append"); \
                exit(1); \
            } \
        } \
        (pda)->items[(pda)->size++] = value; \
    }while(0)

#define da_free(pda) \
    do {\
        free((pda)->items); \
        (pda)->items = NULL; \
        (pda)->size = 0; \
        (pda)->capacity = 0; \
    }while(0)



#endif // MYLIBC_DA_H
