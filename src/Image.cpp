//
// Created by scurab on 01/04/17.
//

#include <cstring>
#include "Image.hpp"
#include "Errors.h"

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



RawData Image::getRawData() {
    RawData r;
    r.data = mImageData;
    r.metaData = mMetaData;
    return r;
}

void Image::setPixels(int *target, int targetComponentsPerPixel) {
    if (mImageData != NULL && target != NULL) {
        RawData data = getRawData();
        if (mComponentsPerPixel == targetComponentsPerPixel) {
            memcpy(target, data.data, (size_t) (data.metaData.pixelCount() * mComponentsPerPixel));
        } else if (mComponentsPerPixel == RGB && targetComponentsPerPixel == RGBA) {
            unsigned char a, b, c;
            unsigned char *rawData = (unsigned char *) mImageData;
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
            if(mComponentsPerPixel == RGBA) {
                const int tmp = ((int *) mImageData)[start];
                next = start;
                do {
                    i = (next % h) * w + next / h;
                    ((int *) mImageData)[next] = (i == start) ? tmp : ((int *) mImageData)[i];
                    next = i;
                } while (next > start);
            } else if(mComponentsPerPixel == RGB) {
                unsigned char * data = ((unsigned char *) mImageData);
                // const int tmp = ((int *) mRawData)[start];
                memcpy(&rgbTmp, data + (start * mComponentsPerPixel), mComponentsPerPixel * sizeof(unsigned char));
                next = start;
                do {
                    i = (next % h) * w + next / h;
                    //((int *) mRawData)[next] = (i == start) ? tmp : ((int *) mRawData)[i];
                    if (i == start) {
                        int realOffset = (next * mComponentsPerPixel);
                        memcpy(data + realOffset, &rgbTmp, mComponentsPerPixel * sizeof(unsigned char));
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

void Image::setRawData(void *data, int w, int h, int componentsPerPixel) {
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
    if (mComponentsPerPixel == RGBA) {
        int* data = (int *) mImageData;
        data[src] ^= data[dst];
        data[dst] ^= data[src];
        data[src] ^= data[dst];
    } else if (mComponentsPerPixel == RGB) {
        unsigned char* data = (unsigned char *) mImageData;
        src *= mComponentsPerPixel;
        dst *= mComponentsPerPixel;
        for (int i = 0; i < mComponentsPerPixel; i++) {
            std::swap(data[src + i], data[dst + i]);
        }
    }
}

void Image::copy(int src, int dst) {
    if (mComponentsPerPixel == RGBA) {
        int *data = (int *) mImageData;
        data[dst] = data[src];
    } else {
        unsigned char *data = (unsigned char *) mImageData;
        src *= mComponentsPerPixel;
        dst *= mComponentsPerPixel;
        memcpy(&data[dst], &data[src], (size_t) mComponentsPerPixel);
    }
}

void Image::clearMetaData() {
    int componentsPerPixel = mComponentsPerPixel;
    mMetaData = {};
    mComponentsPerPixel = componentsPerPixel;
}

IOResult Image::loadImage(ImageProcessor &processor, const char *path) {
    releaseRawData();
    clearMetaData();
    IOResult result = processor.loadImage(path, mComponentsPerPixel, mLastError);
    if (NO_ERR == result.result) {
        mImageData = result.data;
        mMetaData = result.metaData;
    }
    return result;
}

int Image::saveImage(ImageProcessor &processor, const char *path) {
    if (mImageData == nullptr) {
        return NO_DATA;
    }
    InputData input;
    input.data = mImageData;
    input.quality = 100;
    input.metaData = mMetaData;
    input.componentsPerPixel = mComponentsPerPixel;
    return processor.saveImage(path, input);
}
