//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/JPEGImage.hpp"
#include "../src/Errors.h"
#include "jerror.h"

#define TEST_ASSET(filepath) "../test/" filepath
#define JPEG_1X1_PX TEST_ASSET("assets/red.jpg")
#define JPEG_3X1_PX TEST_ASSET("assets/3pixels.jpg")
#define JPEG_INVALID TEST_ASSET("assets/invalid.jpg")


TEST(JPEGImage, LoadingImage) {
    string file = JPEG_1X1_PX;
    JPEGImage image;
    int v = image.loadImage(file.c_str());
    ASSERT_EQ(NO_ERR, v);
}

TEST(JPEGImage, MetaDataImageSize) {
    string f = JPEG_3X1_PX;
    JPEGImage image;
    image.loadImage(f.c_str());
    ImageMetaData data = image.getMetaData();
    ASSERT_EQ(1, data.imageHeight);
    ASSERT_EQ(3, data.imageWidth);
}

TEST(JPEGImage, LoadInvalidImage) {
    string f = JPEG_INVALID;
    JPEGImage image;
    int result = image.loadImage(f.c_str());
    ASSERT_EQ(JERR_NO_SOI, result);
    string err = image.getAndClearLastError();
    string err2 = image.getAndClearLastError();
    ASSERT_EQ(0, err2.length());
}

TEST(JPEGImage, LoadRawData) {
    string f = JPEG_3X1_PX;
    JPEGImage image;
    image.loadImage(f.c_str());
    RawData raw = image.getRawData();
    int *ptr = raw.rawData;
    ASSERT_EQ(3, raw.size);
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
    JPEGImage image;
    image.loadImage(f.c_str());
    f = JPEG_INVALID;
    image.loadImage(f.c_str());
    RawData data = image.getRawData();
    ASSERT_EQ(nullptr, data.rawData);
    ASSERT_EQ(0, data.size);
}

TEST(JPEGImage, SetPixels) {
    string f = JPEG_3X1_PX;
    JPEGImage image;
    image.loadImage(f.c_str());
    int data[3];
    int size = sizeof(data);
    memset(&data, 0, size);
    image.setPixels(&data[0]);

    int *ptr = image.getRawData().rawData;
    ASSERT_EQ(ptr[0], data[0]);
    ASSERT_EQ(ptr[1], data[1]);
    ASSERT_EQ(ptr[2], data[2]);
}