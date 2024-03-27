#ifndef CHR_UTILS_H
#define CHR_UTILS_H

char *mkstr(const char *str);
char *strToHex(const char *instr);
char **strToHexChunk(const char *instr, int *nrChunks);

#endif // CHR_UTILS_H
