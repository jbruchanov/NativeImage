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
    string err = image.getLastError();
    ASSERT_EQ(JERR_NO_SOI, result);
}