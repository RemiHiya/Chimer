#include "include/types.h"
#include <string.h>

int typeNameToInt(const char *name) {
    unsigned int t = 0;
    int i;
    int len = strlen(name);

    for (i=0; i<len; ++i) {
        t += name[i];
    }
    return t;
}
