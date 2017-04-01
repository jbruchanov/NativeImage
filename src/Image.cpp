//
// Created by scurab on 01/04/17.
//

#include <cstring>
#include "Image.h"

Image::Image() {
    mMetaData = {};
    mRawData = nullptr;
}

Image::~Image() {
    releaseRawData();
}

ImageMetaData Image::getMetaData() {
    return mMetaData;
}

void Image::releaseRawData() {
    if (mRawData != 0) {
        delete[] mRawData;
    }
    mRawData = nullptr;
}



RawData Image::getRawData() {
    RawData r;
    r.rawData = mRawData;
    r.size = mMetaData.imageWidth * mMetaData.imageHeight;
    return r;
}

void Image::setPixels(int *target) {
    if (mRawData != NULL && target != NULL) {
        RawData data = getRawData();
        memcpy(target, data.rawData, data.size * sizeof(int));
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
    for (unsigned int start = 0; start <= w * h - 1; ++start) {
        next = start;
        i = 0;
        do {
            ++i;
            next = (next % h) * w + next / h;
        } while (next > start);

        if (next >= start && i != 1) {
            const int tmp = mRawData[start];
            next = start;
            do {
                i = (next % h) * w + next / h;
                mRawData[next] = (i == start) ? tmp : mRawData[i];
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
            swap(mRawData[is], mRawData[it]);
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
    int p;
    for (int i = 0, z = len - 1; i < z; i++, z--) {
        p = mRawData[i];
        mRawData[i] = mRawData[z];
        mRawData[z] = p;
    }
}

void Image::setRawData(int *data, int w, int h) {
    releaseRawData();
    mRawData = data;
    mMetaData.imageWidth = w;
    mMetaData.imageHeight = h;
    mMetaData.componentSize = 4;
}

string Image::getAndClearLastError() {
    string err = string(mLastError);
    int size = sizeof(mLastError);
    memset(mLastError, '\0', size);
    return err;
}
