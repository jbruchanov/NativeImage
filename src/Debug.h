//
// Created by scurab on 05/04/17.
//

#ifndef NATIVEIMAGEAPP_DEBUG_H
#define NATIVEIMAGEAPP_DEBUG_H


class Debug {
public:
    static void printTable(const char *prefix, unsigned char *table, int stride, int imWidth, int components);

private:
    Debug();
};


#endif //NATIVEIMAGEAPP_DEBUG_H
