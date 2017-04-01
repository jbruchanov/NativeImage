//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_JPEGIMAGE_H
#define NATIVEIMAGEAPP_JPEGIMAGE_H

#define BITMAP_COLOR(color) ((0xFF00FF00 & color) | (0x00FF0000 & color) >> 16  | (0x000000FF & color) << 16)

#include <string>
#include "ImageMetaData.h"
#include "jpeglib.h"

using namespace std;

struct RawData {
    int* rawData;
    int size;
};

class JPEGImage {

private:
    ImageMetaData mMetaData;
    char mLastError[JMSG_LENGTH_MAX];
    int *mRawData;

    void releaseRawData();

public:
    JPEGImage();
    ~JPEGImage();

    int loadImage(const char* path);
    int saveImage(const char* path, int quality);
    ImageMetaData getMetaData();
    string getAndClearLastError();

    int storeRawData(JSAMPROW row, int stride, int pixelIndex);

    RawData getRawData();
    void setRawData(int *data, int w, int h);

    void setPixels(int* target);
    void rotate90();
    void rotate180();
};


#endif //NATIVEIMAGEAPP_JPEGIMAGE_H
