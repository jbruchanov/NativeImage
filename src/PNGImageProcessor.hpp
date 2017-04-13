//
// Created by scurab on 01/04/17.
//


#include "Image.hpp"

#ifndef NATIVEIMAGEAPP_PNGIMAGE_H
#define NATIVEIMAGEAPP_PNGIMAGE_H

#define BIT_DEPTH 8


class PNGImageProcessor : public ImageProcessor {
public:
    virtual IOResult loadImage(const char *path, int componentsPerPixel, char *err);
    virtual int saveImage(const char* path, InputData &inputData);
    /**
     * Move pixels in data for proper RGBA (data size and filling must be already prepared)
     * this is just copying pixels!
     * @param data
     * @param width
     * @param height
     * @return
     */
    static void addAlpha(bytep_t *data, int width, int height);
};


#endif //NATIVEIMAGEAPP_PNGIMAGE_H
