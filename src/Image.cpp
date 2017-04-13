//
// Created by scurab on 01/04/17.
//

#include "../json11/json11.hpp"
#include <cstring>
#include <cmath>
#include "Image.hpp"
#include "Errors.h"
#include "LogHelper.h"

#define CHAR_SIZE sizeof(bytep_t)

using namespace json11;

Image::Image(int componentsPerPixel) {
    if (!(componentsPerPixel == RGBA || componentsPerPixel == RGB)) {
        throw "Only 4 or 3 for componentsPerPixel";
    }
    mComponentsPerPixel = componentsPerPixel;
    mMetaData = {};
    mImageData = nullptr;
}

Image::~Image() {
    releaseRawData();
}

ImageMetaData Image::getMetaData() {
    return mMetaData;
}

void Image::releaseRawData() {
    if (mImageData != nullptr) {
        free(mImageData);
    }
    mImageData = nullptr;
}

ImageData Image::getImageData() {
    ImageData r;
    r.data = mImageData;
    r.metaData = mMetaData;
    return r;
}

void Image::setPixels(int *target) {
    this->setPixels(target, 0, 0, mMetaData.imageWidth, mMetaData.imageHeight);
}

void Image::setPixels(int* target, int offsetX, int offsetY, int width, int height) {
    if (mImageData != NULL && target != NULL) {
        int i = 0;
        bytep_t a, b, c, z;
        bytep_t *rawData = mImageData;
        for (int y = offsetY; y < (offsetY + height); y++) {
            for (int x = offsetX; x < (offsetX + width); x++) {
                int startIndex = (y * mMetaData.imageWidth + x) * mComponentsPerPixel;
                z = (mComponentsPerPixel == RGB ? (bytep_t)0xFF : rawData[startIndex++]);
                a = rawData[startIndex++];
                b = rawData[startIndex++];
                c = rawData[startIndex];
                target[i] = z << 24 | a << 16 | b << 8 | c << 0;
                i++;
            }
        }
    }
}

void Image::setPixelsScale(int *target, int width, int height) {
    int w1 = mMetaData.imageWidth, h1 = mMetaData.imageHeight;
    int w2 = width, h2 = height;
    double x_ratio = w1 / (double) w2;
    double y_ratio = h1 / (double) h2;
    double px, py;
    for (int y = 0; y < h2; y++) {
        for (int x = 0; x < w2; x++) {
            px = floor(x * x_ratio);
            py = floor(y * y_ratio);
            int srcOffset = (int) (((py * w1) + px) * mComponentsPerPixel);
            int dstOffset = ARRAY_INDEX(x, y, w2);
            int v;
            if (mComponentsPerPixel == RGBA) {
                v = ((int)mImageData[srcOffset + 0]) << 24 | ((int)mImageData[srcOffset + 1]) << 16 | ((int)mImageData[srcOffset + 2]) << 8 | ((int)mImageData[srcOffset + 3]) << 0;
            } else {
                v = 0xFF << 24 | ((int)mImageData[srcOffset + 0]) << 16 | ((int)mImageData[srcOffset + 1]) << 8 | ((int)mImageData[srcOffset + 2]) << 0;
            }
            target[dstOffset] = v;
        }
    }
}


void Image::rotate90(bool fast) {
    ImageMetaData metaData = getMetaData();
    int w = mMetaData.imageWidth;
    int h = mMetaData.imageHeight;
    int is, it;

    if (fast) {
        bytep_t *temp = (bytep_t *) malloc((size_t) (metaData.imageWidth * metaData.imageHeight * mComponentsPerPixel));
        memset(temp, 0, (metaData.imageWidth * metaData.imageHeight * mComponentsPerPixel));
        //transpose
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                is = ARRAY_INDEX(x, y, w);
                it = ARRAY_INDEX(y, x, h);
                memcpy(temp + (it * mComponentsPerPixel), mImageData + (is * mComponentsPerPixel),
                       (size_t) mComponentsPerPixel);
            }
        }
        releaseRawData();
        mImageData = temp;
    } else {
        //http://softwareengineering.stackexchange.com/questions/271713/transpose-a-matrix-without-a-buffering-one
        //transpose
        unsigned int next = 0;
        unsigned int i = 0;
        bytep_t rgbTmp[mComponentsPerPixel];//needed only for non 4/px
        memset(rgbTmp, 0, sizeof(rgbTmp));

        for (unsigned int start = 0; start <= w * h - 1; ++start) {
            next = start;
            i = 0;
            do {
                ++i;
                next = (next % h) * w + next / h;
            } while (next > start);

            if (next >= start && i != 1) {
               /* following code doesn't make it significantly faster for 4bytes
                * const int tmp = ((int *) mImageData)[start];
                    next = start;
                    do {
                        i = (next % h) * w + next / h;
                        ((int *) mImageData)[next] = (i == start) ? tmp : ((int *) mImageData)[i];
                        next = i;
                    } while (next > start);
                */
                memcpy(&rgbTmp, mImageData + (start * mComponentsPerPixel),
                       mComponentsPerPixel * CHAR_SIZE);
                next = start;
                do {
                    i = (next % h) * w + next / h;
                    //((int *) mRawData)[next] = (i == start) ? tmp : ((int *) mRawData)[i];
                    if (i == start) {
                        int realOffset = (next * mComponentsPerPixel);
                        memcpy(mImageData + realOffset, &rgbTmp, mComponentsPerPixel * CHAR_SIZE);
                    } else {
                        copy(i, next);
                    }
                    next = i;
                } while (next > start);
            }
        }
    }

    h = metaData.imageWidth;
    w = metaData.imageHeight;

    //h-flip
    for (int y = 0; y < h; y++) {
        for (int x = 0, l = w - 1; x < l; x++, l--) {
            is = ARRAY_INDEX(x, y, w);
            it = ARRAY_INDEX(l, y, w);
            swap(is, it);
        }
    }
    mMetaData.imageWidth = w;
    mMetaData.imageHeight = h;
}

void Image::rotate180() {
    ImageMetaData metaData = getMetaData();
    int w = metaData.imageWidth;
    int h = metaData.imageHeight;
    const int len = w * h;
    for (int i = 0, z = len - 1; i < z; i++, z--) {
        swap(i, z);
    }
}

void Image::setRawData(bytep_t *data, int w, int h, int componentsPerPixel) {
    if (!(componentsPerPixel == RGBA || componentsPerPixel == RGB)) {
        throw "Only 4 or 3 for componentsPerPixel";
    }
    releaseRawData();
    mImageData = data;
    mMetaData.imageWidth = w;
    mMetaData.imageHeight = h;
    mComponentsPerPixel = componentsPerPixel;
}

string Image::getAndClearLastError() {
    string err = string(mLastError);
    int size = sizeof(mLastError);
    memset(mLastError, '\0', size);
    return err;
}

inline void Image::swap(int src, int dst) {
    bytep_t *data = mImageData;
    src *= mComponentsPerPixel;
    dst *= mComponentsPerPixel;
    for (int i = 0; i < mComponentsPerPixel; i++) {
        std::swap(data[src + i], data[dst + i]);
    }
}

inline void Image::copy(int src, int dst) {
    bytep_t *data = mImageData;
    src *= mComponentsPerPixel;
    dst *= mComponentsPerPixel;
    memcpy(&data[dst], &mImageData[src], (size_t) mComponentsPerPixel);
}

void Image::clearMetaData() {
    int componentsPerPixel = mComponentsPerPixel;
    mMetaData = {};
    mComponentsPerPixel = componentsPerPixel;
}

IOResult Image::loadImage(ImageProcessor *processor, const char *path) {
    releaseRawData();
    clearMetaData();
    IOResult result = processor->loadImage(path, mComponentsPerPixel, mLastError);
    if (NO_ERR == result.result) {
        mImageData = result.data;
        mMetaData = result.metaData;
    }
    return result;
}

int Image::saveImage(ImageProcessor *processor, const char *path, json11::Json *saveArgs) {
    if (mImageData == nullptr) {
        return NO_DATA;
    }
    InputData input;
    input.data = mImageData;
    input.quality = 85;
    if (saveArgs != nullptr && saveArgs->is_object()) {
        json11::Json quality = (*saveArgs)["jpegQuality"];
        if (quality.is_number()) {
            int value = quality.int_value();
            input.quality = max(0, min(100, value));
        }
    }
    input.metaData = mMetaData;
    input.componentsPerPixel = mComponentsPerPixel;
    return processor->saveImage(path, input);
}

int Image::saveImage(ImageProcessor *processor, const char *path) {
    return this->saveImage(processor, path, nullptr);
}

int Image::getComponentsPerPixel() {
    return mComponentsPerPixel;
}

int Image::applyFilter(EffectFunction &eff, Json &args) {
    const EffectResult er = eff(mImageData, mMetaData.imageWidth, mMetaData.imageHeight,
                                    mComponentsPerPixel, &args);
    if (NO_ERR == er.result) {
        mImageData = er.data;
        mMetaData = er.metaData;
        mComponentsPerPixel = er.componentsPerPixel;
    }
    return er.result;
}