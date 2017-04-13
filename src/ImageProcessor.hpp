//
// Created by scurab on 04/04/17.
//
#include "Types.h"
#include "ImageMetaData.h"
#include "../json11/json11.hpp"
#ifndef NATIVEIMAGEAPP_IMAGEPROCESSOR_H
#define NATIVEIMAGEAPP_IMAGEPROCESSOR_H

using namespace std;
using namespace json11;

struct ImageData {
    bytep_t* data;
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
    /**
     * Remove alpha channel from data
     * @param data
     * @param width
     * @param height
     * @param componentsPerPixel (actual bytes/Px must be 4, otherwise nothing)
     * @return new pointer for data (as it's data shrinking, it should be same (not guaranteed))
     */
    static bytep_t* removeAlpha(bytep_t *data, int width, int height, int componentsPerPixel);
};

#endif //NATIVEIMAGEAPP_IMAGEPROCESSOR_H
