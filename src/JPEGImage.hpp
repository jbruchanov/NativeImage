//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_JPEGIMAGE_H
#define NATIVEIMAGEAPP_JPEGIMAGE_H

#include <string>
#include "ImageMetaData.h"

using namespace std;

class JPEGImage {

private:
    string mFileName;
    ImageMetaData mMetaData;

public:
    JPEGImage();

    int loadImage(const char* path);
    ImageMetaData getMetaData();
    int test(int a, int b);
};


#endif //NATIVEIMAGEAPP_JPEGIMAGE_H
