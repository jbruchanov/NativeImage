//
// Created by scurab on 04/04/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/Errors.h"
#include "TestHelper.h"
#include "../src/ImageProcessor.h"
#include "../src/Image.hpp"
#include "../src/JpegImageProcessor.h"
#include "Assets.h"

typedef unsigned char *string1;
typedef string1 string2;
static JpegImageProcessor prc;

TEST(ImageJPEG4, LoadingImage) {
    string f = JPEG_1X1_PX;
    Image image(4);
    IOResult ior = image.loadImage(prc, f.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
}

TEST(ImageJPEG4, MetaDataImageSize) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(prc, f.c_str());
    ImageMetaData data = image.getMetaData();
    ASSERT_EQ(1, data.imageHeight);
    ASSERT_EQ(3, data.imageWidth);
}

TEST(ImageJPEG4, LoadInvalidImage) {
    string f = JPEG_INVALID;
    Image image(4);
    IOResult ior = image.loadImage(prc, f.c_str());
    ASSERT_NE(NO_ERR, ior.result);
    string err = image.getAndClearLastError();
    string err2 = image.getAndClearLastError();
    ASSERT_EQ(0, err2.length());
}

TEST(ImageJPEG4, LoadRawData) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(prc, f.c_str());
    RawData raw = image.getRawData();
    int *ptr = (int *) raw.data;
    ASSERT_EQ(3, raw.metaData.pixelCount());
    int p1 = ptr[0];
    int p2 = ptr[1];
    int p3 = ptr[2];

    int c1 = BITMAP_COLOR(p1);
    int c2 = BITMAP_COLOR(p2);
    int c3 = BITMAP_COLOR(p3);

    ASSERT_EQ(0xFFA42725, c1);
    ASSERT_EQ(0xFFFFDAD8, c2);
    ASSERT_EQ(0xFF000000, c3);
}

TEST(ImageJPEG4, FreesMemoryOnNewLoad) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(prc, f.c_str());
    f = JPEG_INVALID;
    image.loadImage(prc, f.c_str());
    RawData data = image.getRawData();
    ASSERT_EQ(nullptr, data.data);
    ASSERT_EQ(0, data.metaData.pixelCount());
}

TEST(ImageJPEG4, SetPixels) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(prc, f.c_str());
    int data[3];
    int size = sizeof(data);
    memset(&data, 0, size);
    image.setPixels(data, 4);

    int *ptr = (int *) image.getRawData().data;
    ASSERT_EQ(ptr[0], data[0]);
    ASSERT_EQ(ptr[1], data[1]);
    ASSERT_EQ(ptr[2], data[2]);
}

TEST(ImageJPEG4, Rotate180) {
    Image image(4);
    string2 imageData = new unsigned char[48]
            {11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44,
             51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84,
             91, 92, 93, 94, 101, 102, 103, 104, 111, 112, 113, 114, 121, 122, 123, 124};

    unsigned char imageDataExpected[] = {121, 122, 123, 124, 111, 112, 113, 114, 101, 102, 103, 104, 91, 92, 93, 94,
                                         81, 82, 83, 84, 71, 72, 73, 74, 61, 62, 63, 64, 51, 52, 53, 54,
                                         41, 42, 43, 44, 31, 32, 33, 34, 21, 22, 23, 24, 11, 12, 13, 14};

    image.setRawData(imageData, 4, 3, 4);
    image.rotate180();

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_1) {
    Image image(4);
    unsigned char *imageData = new unsigned char[16]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44};

    unsigned char imageDataExpected[] = {11, 12, 13, 14,
                                         21, 22, 23, 24,
                                         31, 32, 33, 34,
                                         41, 42, 43, 44};

    image.setRawData(imageData, 4, 1, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_2) {
    Image image(4);
    unsigned char *imageData = new unsigned char[24]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34,
                                                     41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64};

    unsigned char imageDataExpected[] = {41, 42, 43, 44, 11, 12, 13, 14,
                                         51, 52, 53, 54, 21, 22, 23, 24,
                                         61, 62, 63, 64, 31, 32, 33, 34};

    image.setRawData(imageData, 3, 2, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_3) {
    Image image(4);
    unsigned char *imageData = new unsigned char[48]
            {11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44,
             51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84,
             91, 92, 93, 94, 101, 102, 103, 104, 111, 112, 113, 114, 121, 122, 123, 124};

    unsigned char imageDataExpected[] = {91, 92, 93, 94, 51, 52, 53, 54, 11, 12, 13, 14,
                                         101, 102, 103, 104, 61, 62, 63, 64, 21, 22, 23, 24,
                                         111, 112, 113, 114, 71, 72, 73, 74, 31, 32, 33, 34,
                                         121, 122, 123, 124, 81, 82, 83, 84, 41, 42, 43, 44};

    image.setRawData(imageData, 4, 3, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, SaveImage) {
    Image image(4);
    string f = JPEG_SAMPLE_ASSET;
    image.loadImage(prc, f.c_str());
    const char *path = "pokus4.jpg";
    image.saveImage(prc, path);

    Image image2(4);
    image2.loadImage(prc, f.c_str());
    const ImageMetaData metaData = image.getMetaData();
    ASSERT_EQ(image.getMetaData().imageWidth, metaData.imageWidth);
    ASSERT_EQ(image.getMetaData().imageHeight, metaData.imageHeight);
    remove(path);
}