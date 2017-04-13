//
// Created by scurab on 05/04/17.
//

#ifndef NATIVEIMAGEAPP_DEBUG_H
#define NATIVEIMAGEAPP_DEBUG_H

#include "Types.h"

class Debug {
public:
    static void printTable(const char *prefix, bytep_t *table, int stride, int imWidth, int components);

private:
    Debug(){};
};


#endif //NATIVEIMAGEAPP_DEBUG_H
