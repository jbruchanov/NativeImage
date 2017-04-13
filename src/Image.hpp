//
// Created by scurab on 01/04/17.
//

#ifndef NATIVEIMAGEAPP_IMAGE_H
#define NATIVEIMAGEAPP_IMAGE_H

#define BITMAP_COLOR(color) ((0xFF00FF00 & color) | (0x00FF0000 & color) >> 16  | (0x000000FF & color) << 16)
#define ARRAY_INDEX(x, y, w) (x + (y * w))

#include "Types.h"
#include <string>
#include "ImageMetaData.h"
#include "ImageProcessor.hpp"
#include "Effect.h"

class Image {

protected:
    ImageMetaData mMetaData;
    char mLastError[200];
    unsigned char *mImageData;
    void releaseRawData();
    void clearMetaData();
    int mComponentsPerPixel;

public:
    Image(int componentsPerPixel);
    virtual ~Image();

    IOResult loadImage(ImageProcessor *processor, const char *path);
    int saveImage(ImageProcessor *processor, const char *path);
    int saveImage(ImageProcessor *processor, const char *path, Json *saveArgs);

    ImageMetaData getMetaData();
    string getAndClearLastError();

    ImageData getImageData();
    void setRawData(bytep_t *data, int w, int h, int componentsPerPixel);
    void swap(int src, int dst);
    void copy(int src, int dst);

    void setPixels(int* target);
    void setPixels(int* target, int offsetX, int offsetY, int width, int height);
    void setPixelsScale(int* target, int width, int height);
    void rotate90(bool fast);
    void rotate180();

    int getComponentsPerPixel();

    int applyFilter(EffectFunction &eff, Json &args);
};


#endif //NATIVEIMAGEAPP_IMAGE_H
