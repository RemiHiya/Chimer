#include "include/utils.h"
#include "include/list.h"
#include "include/macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assertNotNull(void *x) {
    if (!!x) return;
    printf("[ASSERT FAILED]: %p\n", x);
    exit(1);
}

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

list_T *strToHexChunk(const char *instr) {
    list_T *list = initList(sizeof(char*));

    unsigned int i = 0;
    char *tmp = calloc(1, sizeof(char));
    while (instr[i] != '\0') {
        tmp = realloc(tmp, (strlen(tmp) + 2) * sizeof(char));
        strcat(tmp, (char[]) {instr[i], 0});

        if ((i>0 && (i-1) % 4 == 0) || instr[i] == '\n' || instr[i] == '\t') {
            char *hexstr = strToHex(tmp);
            free(tmp);
            listPush(list, hexstr);
            tmp = calloc(1, sizeof(char));
        }
        ++i;
    }

    return list;
}

char *strFormat(char *instr) {
    char *newstr = calloc(1, sizeof(char));
    unsigned int len = strlen(instr);
    int i = 0;
    while (instr[i] != '\0' && i<len) {
        newstr = realloc(newstr, (strlen(newstr) + 2) * sizeof(char));

        if (instr[i] == '\\') {
            char escape = strToEscapeSeq((char[]) {instr[i], instr[MIN(i+1, len)], 0});
            strcat(newstr, (char[]) {escape, 0});
            i+=2;
        }
        else {
            strcat(newstr, (char[]) {instr[i], 0});
            ++i;
        }
    }
    return newstr;
}

char strToEscapeSeq(const char *instr) {
    if (strlen(instr) <= 1) return 0;
    if (instr[0] != '\\') return 0;

    char inC = instr[1];

    switch (inC) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return inC;
        default: return inC;
    }
    return inC;
}
