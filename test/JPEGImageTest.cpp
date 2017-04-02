//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/JPEGImage.hpp"
#include "../src/Errors.h"
#include "jerror.h"
#include "TestHelper.h"

#define JPEG_1X1_PX TEST_ASSET("assets/red.jpg")
#define JPEG_3X1_PX TEST_ASSET("assets/3pixels.jpg")
#define JPEG_INVALID TEST_ASSET("assets/invalid.jpg")
#define JPEG_SAMPLE_ASSET TEST_ASSET("assets/test.jpg")


TEST(JPEGImage, LoadingImage) {
    string file = JPEG_1X1_PX;
    JPEGImage image(4);
    int v = image.loadImage(file.c_str());
    ASSERT_EQ(NO_ERR, v);
}

TEST(JPEGImage, MetaDataImageSize) {
    string f = JPEG_3X1_PX;
    JPEGImage image(4);
    image.loadImage(f.c_str());
    ImageMetaData data = image.getMetaData();
    ASSERT_EQ(1, data.imageHeight);
    ASSERT_EQ(3, data.imageWidth);
}

TEST(JPEGImage, LoadInvalidImage) {
    string f = JPEG_INVALID;
    JPEGImage image(4);
    int result = image.loadImage(f.c_str());
    ASSERT_EQ(JERR_NO_SOI, result);
    string err = image.getAndClearLastError();
    string err2 = image.getAndClearLastError();
    ASSERT_EQ(0, err2.length());
}

TEST(JPEGImage, LoadRawData) {
    string f = JPEG_3X1_PX;
    JPEGImage image(4);
    image.loadImage(f.c_str());
    RawData raw = image.getRawData();
    int *ptr = (int *) raw.rawData;
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

TEST(JPEGImage, FreesMemoryOnNewLoad) {
    string f = JPEG_3X1_PX;
    JPEGImage image(4);
    image.loadImage(f.c_str());
    f = JPEG_INVALID;
    image.loadImage(f.c_str());
    RawData data = image.getRawData();
    ASSERT_EQ(nullptr, data.rawData);
    ASSERT_EQ(0, data.metaData.pixelCount());
}

TEST(JPEGImage, SetPixels) {
    string f = JPEG_3X1_PX;
    JPEGImage image(4);
    image.loadImage(f.c_str());
    int data[3];
    int size = sizeof(data);
    memset(&data, 0, size);
    image.setPixels(&data[0]);

    int *ptr = (int *) image.getRawData().rawData;
    ASSERT_EQ(ptr[0], data[0]);
    ASSERT_EQ(ptr[1], data[1]);
    ASSERT_EQ(ptr[2], data[2]);
}

TEST(JPEGImage, Rotate180) {
    JPEGImage image(4);
    int *imageData = new int[12]{1, 2, 3, 4,
                                 5, 6, 7, 8,
                                 9, 10, 11, 12};

    int imageDataExpected[] = {12, 11, 10, 9,
                               8, 7, 6, 5,
                               4, 3, 2, 1};

    image.setRawData(imageData, 4, 3, 4);
    image.rotate180();

    for (int i = 0, n = sizeof(imageData) / sizeof(int); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(JPEGImage, Rotate90_1) {
    JPEGImage image(4);
    int *imageData = new int[4]{1, 2, 3, 4};

    int imageDataExpected[] = {1,
                               2,
                               3,
                               4};

    image.setRawData(imageData, 4, 1, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(int); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(JPEGImage, Rotate90_2) {
    JPEGImage image(4);
    int *imageData = new int[6]{1, 2, 3,
                                4, 5, 6};

    int imageDataExpected[] = {4, 1,
                               5, 2,
                               6, 3};

    image.setRawData(imageData, 3, 2, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(int); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(JPEGImage, Rotate90_3) {
    JPEGImage image(4);
    int *imageData = new int[12]{1, 2, 3, 4,
                                 5, 6, 7, 8,
                                 9, 10, 11, 12};

    int imageDataExpected[] = {9, 5, 1,
                               10, 6, 2,
                               11, 7, 3,
                               12, 8, 4};

    image.setRawData(imageData, 4, 3, 4);
    image.rotate90();

    for (int i = 0, n = sizeof(imageData) / sizeof(int); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(JPEGImage, SaveImage) {
    JPEGImage image(4);
    string f = JPEG_SAMPLE_ASSET;
    image.loadImage(f.c_str());
    const char *path = "pokus.jpg";
    image.saveImage(path, 85);
    
    JPEGImage image2(4);
    image2.loadImage(path);
    const ImageMetaData metaData = image.getMetaData();
    ASSERT_EQ(image.getMetaData().imageWidth, metaData.imageWidth);
    ASSERT_EQ(image.getMetaData().imageHeight, metaData.imageHeight);
    remove(path);
}