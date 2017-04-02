//
// Created by scurab on 01/04/17.
//

#include <cstring>
#include "Image.h"

Image::Image(int componentsPerPixel) {
    if (!(componentsPerPixel == 4 || componentsPerPixel == 3)) {
        throw "Only 4 or 3 for componentsPerPixel";
    }
    mMetaData = {};
    mMetaData.componentsPerPixel = componentsPerPixel;
    mRawData = nullptr;
}

Image::~Image() {
    releaseRawData();
}

ImageMetaData Image::getMetaData() {
    return mMetaData;
}

void Image::releaseRawData() {
    if (mRawData != nullptr) {
        free(mRawData);
    }
    mRawData = nullptr;
}



RawData Image::getRawData() {
    RawData r;
    r.rawData = mRawData;
    r.metaData = mMetaData;
    return r;
}

void Image::setPixels(int *target) {
    if (mRawData != NULL && target != NULL) {
        RawData data = getRawData();
        memcpy(target, data.rawData, (size_t) (data.metaData.pixelCount() * mMetaData.componentsPerPixel));
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
    unsigned char rgb[mMetaData.componentsPerPixel];//needed only for non 4/px
    for (unsigned int start = 0; start <= w * h - 1; ++start) {
        next = start;
        i = 0;
        do {
            ++i;
            next = (next % h) * w + next / h;
        } while (next > start);

        if (next >= start && i != 1) {
            if(mMetaData.componentsPerPixel == 4) {
                const int tmp = ((int *) mRawData)[start];
                next = start;
                do {
                    i = (next % h) * w + next / h;
                    ((int *) mRawData)[next] = (i == start) ? tmp : ((int *) mRawData)[i];
                    next = i;
                } while (next > start);
            } else if(mMetaData.componentsPerPixel == 3) {
                //TODO:
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
    if (!(componentsPerPixel == 4 || componentsPerPixel == 3)) {
        throw "Only 4 or 3 for componentsPerPixel";
    }
    releaseRawData();
    mRawData = data;
    mMetaData.imageWidth = w;
    mMetaData.imageHeight = h;
    mMetaData.componentsPerPixel = componentsPerPixel;
}

string Image::getAndClearLastError() {
    string err = string(mLastError);
    int size = sizeof(mLastError);
    memset(mLastError, '\0', size);
    return err;
}

void Image::swap(int src, int dst) {
    if (mMetaData.componentsPerPixel == 4) {
        int* data = (int *) mRawData;
        data[src] ^= data[dst];
        data[dst] ^= data[src];
        data[src] ^= data[dst];
    } else if (mMetaData.componentsPerPixel == 3) {
        unsigned char* data = (unsigned char *) mRawData;
        src *= mMetaData.componentsPerPixel;
        dst *= mMetaData.componentsPerPixel;
        for (int i = 0; i < mMetaData.componentsPerPixel; i++) {
            std::swap(data[src], data[dst]);
        }
    }
}

void Image::copy(int src, int dst) {
    if (mMetaData.componentsPerPixel == 4) {
        int *data = (int *) mRawData;
        data[dst] = data[src];
    } else {
        unsigned char *data = (unsigned char *) mRawData;
        src *= mMetaData.componentsPerPixel;
        dst *= mMetaData.componentsPerPixel;
        for (int i = 0; i < mMetaData.componentsPerPixel; i++) {
            data[dst] = data[src];
        }
    }
}

void Image::clearMetaData() {
    int componentsPerPixel = mMetaData.componentsPerPixel;
    mMetaData = {};
    mMetaData.componentsPerPixel = componentsPerPixel;
}
