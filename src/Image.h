//
// Created by scurab on 01/04/17.
//

#ifndef NATIVEIMAGEAPP_IMAGE_H
#define NATIVEIMAGEAPP_IMAGE_H

#define BITMAP_COLOR(color) ((0xFF00FF00 & color) | (0x00FF0000 & color) >> 16  | (0x000000FF & color) << 16)
#define ARRAY_INDEX(x, y, w) (x + (y * w))

#include <string>
#include "ImageMetaData.h"

using namespace std;

struct RawData {
    void* rawData;
    ImageMetaData metaData;
};

class Image {

protected:
    ImageMetaData mMetaData;
    char mLastError[200];
    void *mRawData;
    void releaseRawData();
    void clearMetaData();

public:
    Image(int componentsPerPixel);
    virtual ~Image();

    virtual int loadImage(const char* path) = 0;
    virtual int saveImage(const char* path, int quality) = 0;
    ImageMetaData getMetaData();
    string getAndClearLastError();

    RawData getRawData();
    void setRawData(void *data, int w, int h, int componentsPerPixel);
    void swap(int src, int dst);
    void copy(int src, int dst);

    void setPixels(int* target);
    void rotate90();
    void rotate180();
};


#endif //NATIVEIMAGEAPP_IMAGE_H
