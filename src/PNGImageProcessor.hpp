//
// Created by scurab on 01/04/17.
//


#include "Image.hpp"

#ifndef NATIVEIMAGEAPP_PNGIMAGE_H
#define NATIVEIMAGEAPP_PNGIMAGE_H


class PNGImageProcessor : public ImageProcessor {
public:
    virtual IOResult loadImage(const char *path, int componentsPerPixel, char *err);
    virtual int saveImage(const char* path, InputData &inputData);
};


#endif //NATIVEIMAGEAPP_PNGIMAGE_H
