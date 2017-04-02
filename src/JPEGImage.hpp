//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_JPEGIMAGE_H
#define NATIVEIMAGEAPP_JPEGIMAGE_H

#include <string>
#include "Image.h"
#include "ImageMetaData.h"
#include "jpeglib.h"

class JPEGImage : public Image {

public:
    JPEGImage(int componentsPerPixel);

    int loadImage(const char* path);
    int saveImage(const char* path, int quality);

    int storeRawData(JSAMPROW row, int stride, int pixelIndex);
};


#endif //NATIVEIMAGEAPP_JPEGIMAGE_H
