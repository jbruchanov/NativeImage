//
// Created by scurab on 04/04/17.
//
#include <cstring>
#include "ImageProcessor.hpp"
#include "LogHelper.h"

unsigned char *ImageProcessor::removeAlpha(unsigned char *data, int width, int height, int componentsPerPixel) {
    if (data != nullptr && componentsPerPixel == 4) {
        int newComponent = 3;
        long newLen = (long) width * height * newComponent;
        for (int i = 0, l = (width * height) - 1; i < l; i++) {
            long from = (long) (i) * componentsPerPixel;
            long to = (long) i * newComponent;
            memcpy(&data[to], &data[from + 1/*no Alpha*/], (size_t) newComponent);
        }
        void *newPtr = realloc(data, (size_t) newLen);
        if (newPtr == nullptr) {
            LOGE("Unable to realloc!, data has been updated, but array size kept same!");
        } else {
            //i would expect that pointer will be same, as it's shrinking, so no need for changing,
            //but docs seem to be saying that it's not guaranteed
            data = (unsigned char *) newPtr;
        }
    }
    return data;
}
