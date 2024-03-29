#ifndef CHR_UTILS_H
#define CHR_UTILS_H

void assertNotNull(void *x);
char *mkstr(const char *str);
char *strToHex(const char *instr);
char **strToHexChunk(const char *instr, int *nrChunks);
char strToEscapeSeq(const char *instr);
char *strFormat(char *instr);

#endif // CHR_UTILS_H
