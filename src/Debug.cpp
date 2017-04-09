//
// Created by scurab on 05/04/17.
//

#include <cstdio>
#include "Debug.h"

void Debug::printTable(const char *prefix, unsigned char *table, int stride, int imWidth, int components) {
    printf("%s\n", prefix);
    int spaces = 0;
    for (int i = 0; i < stride; i++) {
        if (i > 0 && (i % components == 0)) {
            printf(" ");
            spaces++;
            if (spaces == imWidth) {
                printf("\n");
                spaces = 0;
            }
        }
        printf("%02d", (int)table[i]);//%02x
    }
    printf("\n");
}