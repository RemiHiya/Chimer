#ifndef CHR_LIST_H
#define CHR_LIST_H
#include <stdlib.h>

typedef struct LIST_STRUCT {
    void **items;
    size_t size;
    size_t itemSize;
} list_T;

list_T *initList(size_t itemSize);
void listPush(list_T *list, void *item);

#endif // CHR_LIST_H
