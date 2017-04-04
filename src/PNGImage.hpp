//
// Created by scurab on 01/04/17.
//


#include "Image.hpp"

#ifndef NATIVEIMAGEAPP_PNGIMAGE_H
#define NATIVEIMAGEAPP_PNGIMAGE_H


class PNGImage : public Image {
public:
    PNGImage(int componentsPerPixel);

    int loadImage(const char* path);
    int saveImage(const char* path, int quality);

//    int storeRawData(JSAMPROW row, int stride, int pixelIndex);
};


#endif //NATIVEIMAGEAPP_PNGIMAGE_H
