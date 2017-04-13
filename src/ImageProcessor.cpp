//
// Created by scurab on 04/04/17.
//
#include <cstring>
#include "ImageProcessor.hpp"
#include "LogHelper.h"

bytep_t *ImageProcessor::removeAlpha(bytep_t *data, int width, int height, int componentsPerPixel) {
    if (data != nullptr && componentsPerPixel == RGBA) {
        int newComponent = 3;
        long newLen = (long) width * height * newComponent;
        for (int isrc = 0, idst = 0, n = width * height * RGBA; isrc < n; /*isrc++, idst++*/) {
            isrc++;//alpha
            data[idst++] = data[isrc++];//R
            data[idst++] = data[isrc++];//G
            data[idst++] = data[isrc++];//B => increment here for-i doesn't increment at all
        }
        void *newPtr = realloc(data, (size_t) newLen);
        if (newPtr == nullptr) {
            LOGE("Unable to realloc!, data has been updated, but array size kept same!");
        } else {
            //i would expect that pointer will be same, as it's shrinking, so no need for changing,
            //but docs seem to be saying that it's not guaranteed
            data = (bytep_t *) newPtr;
        }
    }
    return data;
}
