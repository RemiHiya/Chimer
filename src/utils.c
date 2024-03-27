#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mkstr(const char *str) {
    char *outstr = calloc(strlen(str) + 1, sizeof(char));
    strcpy(outstr, str);
    return outstr;
}

char *strToHex(const char *instr) {
    unsigned int len = strlen(instr);
    char *hexstr = calloc(1, sizeof(char));

    for (unsigned int i=0; i<len+1; ++i) {
        char *newstr = calloc(4, sizeof(char));
        sprintf(newstr, "%x", instr[len-i]);
        hexstr = realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
        strcat(hexstr, newstr);
        free(newstr);
    }
    return hexstr;
}

char **strToHexChunk(const char *instr, int *nrChunks) {
    unsigned int len = strlen(instr);
    unsigned int nbchunks = 1 + len/4;
    int *nrptr = nrChunks;
    *nrptr = nbchunks;

    char **strlist = calloc(nbchunks * 5, sizeof(char));

    for (unsigned int i=0; i<(len/4)+1; ++i) {
        char *chunkstr = mkstr(instr + i*4);
        chunkstr = realloc(chunkstr, 4);
        chunkstr[4] = 0;
        char *hexstr = strToHex(chunkstr);
        strlist[i] = hexstr;
    }

    return strlist;
}
