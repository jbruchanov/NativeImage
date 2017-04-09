//
// Created by scurab on 04/04/17.
//
#include "ImageProcessor.hpp"

#ifndef NATIVEIMAGEAPP_JPEGIMAGEPROCESSOR_H
#define NATIVEIMAGEAPP_JPEGIMAGEPROCESSOR_H

class JpegImageProcessor : public ImageProcessor {
public:
    virtual IOResult loadImage(const char *path, int componentsPerPixel, char *err);
    virtual int saveImage(const char* path, InputData &inputData);
};


#endif //NATIVEIMAGEAPP_JPEGIMAGEPROCESSOR_H
