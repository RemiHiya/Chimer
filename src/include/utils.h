#ifndef CHR_UTILS_H
#define CHR_UTILS_H
#include "list.h"

void assertNotNull(void *x);
char *mkstr(const char *str);
char *strToHex(const char *instr);
list_T *strToHexChunk(const char *instr);
char strToEscapeSeq(const char *instr);
char *strFormat(char *instr);

#endif // CHR_UTILS_H
