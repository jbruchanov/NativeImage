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
    ImageMetaData getMetaData();
    string getAndClearLastError();

    void storeRawData(JSAMPROW row, int stride, int rawDataIndex, int *pInt);

    RawData getRawData();
    void setPixels(int* target);
};


#endif //NATIVEIMAGEAPP_JPEGIMAGE_H
