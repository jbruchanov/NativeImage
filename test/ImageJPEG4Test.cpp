//
// Created by scurab on 04/04/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/Errors.h"
#include "../src/ImageProcessor.hpp"
#include "../src/Image.hpp"
#include "../src/JpegImageProcessor.h"
#include "Assets.h"

typedef unsigned char *string1;
typedef string1 string2;
static JpegImageProcessor prc;

TEST(ImageJPEG4, LoadingImage) {
    string f = JPEG_1X1_PX;
    Image image(4);
    IOResult ior = image.loadImage(&prc, f.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
}

TEST(ImageJPEG4, MetaDataImageSize) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(&prc, f.c_str());
    ImageMetaData data = image.getMetaData();
    ASSERT_EQ(1, data.imageHeight);
    ASSERT_EQ(3, data.imageWidth);
}

TEST(ImageJPEG4, LoadInvalidImage) {
    string f = JPEG_INVALID;
    Image image(4);
    IOResult ior = image.loadImage(&prc, f.c_str());
    ASSERT_NE(NO_ERR, ior.result);
    string err = image.getAndClearLastError();
    string err2 = image.getAndClearLastError();
    ASSERT_EQ(0, err2.length());
}

TEST(ImageJPEG4, LoadRawData_1PX) {
    string f = JPEG_1X1_PX;
    Image image(4);
    image.loadImage(&prc, f.c_str());
    ImageData raw = image.getImageData();
    unsigned char *ptr = raw.data;
    ASSERT_EQ(1, raw.metaData.pixelCount());

    unsigned char exp[] = {0xFF, 0x00, 0x00, 0xFE};
    for (int i = 0; i < 4; i++) {
        unsigned char pc = ptr[i];
        ASSERT_EQ(exp[i], pc);
    }
}

TEST(ImageJPEG4, LoadRawData_3PX) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(&prc, f.c_str());
    ImageData raw = image.getImageData();
    unsigned char *ptr = raw.data;
    ASSERT_EQ(3, raw.metaData.pixelCount());
    unsigned char exp[] = {0xFF, 0x25, 0x27, 0xA4,
                           0xFF, 0xD8, 0xDA, 0xFF,
                           0xFF, 0x00, 0x00, 0x00};
    for (int i = 0; i < 12; i++) {
        unsigned char pc = ptr[i];
        ASSERT_EQ(exp[i], pc);
    }
}

TEST(ImageJPEG4, FreesMemoryOnNewLoad) {
    string f = JPEG_3X1_PX;
    Image image(4);
    image.loadImage(&prc, f.c_str());
    f = JPEG_INVALID;
    image.loadImage(&prc, f.c_str());
    ImageData data = image.getImageData();
    ASSERT_EQ(nullptr, data.data);
    ASSERT_EQ(0, data.metaData.pixelCount());
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

TEST(ImageJPEG4, Rotate90_1Slow) {
    Image image(4);
    unsigned char *imageData = new unsigned char[16]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44};

    unsigned char imageDataExpected[] = {11, 12, 13, 14,
                                         21, 22, 23, 24,
                                         31, 32, 33, 34,
                                         41, 42, 43, 44};

    image.setRawData(imageData, 4, 1, 4);
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_1Fast) {
    Image image(4);
    unsigned char *imageData = new unsigned char[16]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44};

    unsigned char imageDataExpected[] = {11, 12, 13, 14,
                                         21, 22, 23, 24,
                                         31, 32, 33, 34,
                                         41, 42, 43, 44};

    image.setRawData(imageData, 4, 1, 4);
    image.rotate90(true);
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_2Slow) {
    Image image(4);
    unsigned char *imageData = new unsigned char[24]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34,
                                                     41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64};

    unsigned char imageDataExpected[] = {41, 42, 43, 44, 11, 12, 13, 14,
                                         51, 52, 53, 54, 21, 22, 23, 24,
                                         61, 62, 63, 64, 31, 32, 33, 34};

    image.setRawData(imageData, 3, 2, 4);
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_2Fast) {
    Image image(4);
    unsigned char *imageData = new unsigned char[24]{11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34,
                                                     41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64};

    unsigned char imageDataExpected[] = {41, 42, 43, 44, 11, 12, 13, 14,
                                         51, 52, 53, 54, 21, 22, 23, 24,
                                         61, 62, 63, 64, 31, 32, 33, 34};

    image.setRawData(imageData, 3, 2, 4);
    image.rotate90(true);
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_3Slow) {
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
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, Rotate90_3Fast) {
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
    image.rotate90(true);

    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG4, SaveImage) {
    Image image(4);
    string f = JPEG_SAMPLE_ASSET;
    image.loadImage(&prc, f.c_str());
    const char *path = "pokus4.jpg";
    image.saveImage(&prc, path);

    Image image2(4);
    image2.loadImage(&prc, f.c_str());
    const ImageMetaData metaData = image.getMetaData();
    ASSERT_EQ(image.getMetaData().imageWidth, metaData.imageWidth);
    ASSERT_EQ(image.getMetaData().imageHeight, metaData.imageHeight);
    remove(path);
}

TEST(ImageJPEG4, SetPixelsCrop) {
    Image image(4);
    unsigned char *imageData = new unsigned char[48]
            {0x11, 0x12, 0x13, 0x14, 0x21, 0x22, 0x23, 0x24, 0x31, 0x32, 0x33, 0x34, 0x41, 0x42, 0x43, 0x44,
             0x51, 0x52, 0x53, 0x54, 0x61, 0x62, 0x63, 0x64, 0x71, 0x72, 0x73, 0x74, 0x81, 0x82, 0x83, 0x84,
             0x91, 0x92, 0x93, 0x94, 0xA1, 0xA2, 0xA3, 0xA4, 0xB1, 0xB2, 0xB3, 0xB4, 0xC1, 0xC2, 0xC3, 0xC4};

    image.setRawData(imageData, 4, 3, 4);

    int target[4];

    //NO BITMAP_COLOR, because it's simple set!
    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 0, 0, 1, 1);
    ASSERT_EQ((0x11121314), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 1, 1, 1, 1);
    ASSERT_EQ((0x61626364), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 2, 2, 1, 1);
    ASSERT_EQ((0xB1B2B3B4), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 2, 2, 2, 1);
    ASSERT_EQ((0xB1B2B3B4), target[0]);
    ASSERT_EQ((0xC1C2C3C4), target[1]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 3, 0, 1, 3);
    ASSERT_EQ((0x41424344), target[0]);
    ASSERT_EQ((0x81828384), target[1]);
    ASSERT_EQ((0xC1C2C3C4), target[2]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 0, 2, 4, 1);
    ASSERT_EQ((0x91929394), target[0]);
    ASSERT_EQ((0xA1A2A3A4), target[1]);
    ASSERT_EQ((0xB1B2B3B4), target[2]);
    ASSERT_EQ((0xC1C2C3C4), target[3]);
}