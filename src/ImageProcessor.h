//
// Created by scurab on 04/04/17.
//
#include "ImageMetaData.h"
#include "../json11/json11.hpp"
#ifndef NATIVEIMAGEAPP_IMAGEPROCESSOR_H
#define NATIVEIMAGEAPP_IMAGEPROCESSOR_H

using namespace std;
using namespace json11;

struct ImageData {
    unsigned char* data;
    ImageMetaData metaData;
};

struct IOResult : ImageData {
    IOResult() {}

    IOResult(int code) {
        result = code;
    }
    int result;
};

struct InputData : ImageData {
    int quality;
    int componentsPerPixel;
};

class ImageProcessor {
public:
    virtual IOResult loadImage(const char *path, int componentsPerPixel, char *err) = 0;
    virtual int saveImage(const char* path, InputData &data) = 0;
};

#endif //NATIVEIMAGEAPP_IMAGEPROCESSOR_H