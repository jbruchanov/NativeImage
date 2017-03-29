//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_JPEGIMAGE_H
#define NATIVEIMAGEAPP_JPEGIMAGE_H

#include <string>
#include "ImageMetaData.h"
#include "jpeglib.h"

using namespace std;

class JPEGImage {

private:
    ImageMetaData mMetaData;
    char mLastError[JMSG_LENGTH_MAX];

public:
    JPEGImage();

    int loadImage(const char* path);
    ImageMetaData getMetaData();
    string getLastError();
};


#endif //NATIVEIMAGEAPP_JPEGIMAGE_H
