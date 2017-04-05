//
// Created by scurab on 01/04/17.
//

#include "../json11/json11.hpp"
#include <cstring>
#include "Image.hpp"
#include "Errors.h"
#include "cstring"

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

void Image::setPixels(int *target, int targetComponentsPerPixel) {
    if (mImageData != NULL && target != NULL) {
        ImageData data = getImageData();
        if (mComponentsPerPixel == targetComponentsPerPixel) {
            memcpy(target, data.data, (size_t) (data.metaData.pixelCount() * mComponentsPerPixel));
        } else if (mComponentsPerPixel == RGB && targetComponentsPerPixel == RGBA) {
            unsigned char a, b, c;
            unsigned char *rawData = mImageData;
            for (int i = 0, l = mMetaData.pixelCount(); i < l; i++) {
                int startIndex = i * mComponentsPerPixel;
                a = rawData[startIndex];
                b = rawData[++startIndex];
                c = rawData[++startIndex];
                target[i] = 0xFF000000 | a << 0 | b << 8 | c << 16;
            }
        }
    }
}

void Image::rotate90() {
    ImageMetaData metaData = getMetaData();
    int w = mMetaData.imageWidth;
    int h = mMetaData.imageHeight;
    int is, it;

    //http://softwareengineering.stackexchange.com/questions/271713/transpose-a-matrix-without-a-buffering-one
    //transpose
    unsigned int next = 0;
    unsigned int i = 0;
    unsigned char rgbTmp[mComponentsPerPixel];//needed only for non 4/px
    memset(rgbTmp, 0, sizeof(rgbTmp));
    for (unsigned int start = 0; start <= w * h - 1; ++start) {
        next = start;
        i = 0;
        do {
            ++i;
            next = (next % h) * w + next / h;
        } while (next > start);

        if (next >= start && i != 1) {
            //algorithm for int
            /*
                const int tmp = ((int *) mImageData)[start];
                next = start;
                do {
                    i = (next % h) * w + next / h;
                    ((int *) mImageData)[next] = (i == start) ? tmp : ((int *) mImageData)[i];
                    next = i;
                } while (next > start);
                unsigned char * data = ((unsigned char *) mImageData);
            */
            memcpy(&rgbTmp, mImageData + (start * mComponentsPerPixel), mComponentsPerPixel * sizeof(unsigned char));
            next = start;
            do {
                i = (next % h) * w + next / h;
                //((int *) mRawData)[next] = (i == start) ? tmp : ((int *) mRawData)[i];
                if (i == start) {
                    int realOffset = (next * mComponentsPerPixel);
                    memcpy(mImageData + realOffset, &rgbTmp, mComponentsPerPixel * sizeof(unsigned char));
                } else {
                    copy(i, next);
                }
                next = i;
            } while (next > start);
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

void Image::setRawData(unsigned char *data, int w, int h, int componentsPerPixel) {
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

void Image::swap(int src, int dst) {
    unsigned char *data = mImageData;
    src *= mComponentsPerPixel;
    dst *= mComponentsPerPixel;
    for (int i = 0; i < mComponentsPerPixel; i++) {
        std::swap(data[src + i], data[dst + i]);
    }
}

void Image::copy(int src, int dst) {
    unsigned char *data = mImageData;
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
