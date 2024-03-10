#include "include/chr.h"
#include "include/macro.h"
#include "include/io.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Missing input file.\n");
        return 1;
    }
    printf("Begin compile.\n");
    chrCompileFile(argv[1]);
}
