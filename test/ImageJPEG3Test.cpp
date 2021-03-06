//
// Created by scurab on 04/04/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/Errors.h"
#include "../src/ImageProcessor.hpp"
#include "../src/Image.hpp"
#include "../src/JpegImageProcessor.h"
#include "Assets.h"
#include "../src/Debug.h"

static JpegImageProcessor prc;

TEST(ImageJPEG3, LoadingImage) {
    string f = JPEG_1X1_PX;
    Image image(3);

    IOResult ior = image.loadImage(&prc, f.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
}

TEST(ImageJPEG3, MetaDataImageSize) {
    string f = JPEG_3X1_PX;
    Image image(3);
    image.loadImage(&prc, f.c_str());
    ImageMetaData data = image.getMetaData();
    ASSERT_EQ(1, data.imageHeight);
    ASSERT_EQ(3, data.imageWidth);
}

TEST(ImageJPEG3, LoadInvalidImage) {
    string f = JPEG_INVALID;
    Image image(3);
    IOResult ior = image.loadImage(&prc, f.c_str());
    ASSERT_NE(NO_ERR, ior.result);
    string err = image.getAndClearLastError();
    string err2 = image.getAndClearLastError();
    ASSERT_EQ(0, err2.length());
}

TEST(ImageJPEG3, LoadRawData) {
    string f = JPEG_3X1_PX;
    Image image(3);
    image.loadImage(&prc, f.c_str());
    ImageData raw = image.getImageData();
    bytep_t *ptr = (bytep_t *) raw.data;
    ASSERT_EQ(3, raw.metaData.pixelCount());

    bytep_t exp[] = {0x25, 0x27, 0xA4,
                           0xD8, 0xDA, 0xFF,
                           0x00, 0x00, 0x00};
    for (int i = 0; i < 9; i++) {
        bytep_t pc = ptr[i];
        ASSERT_EQ(exp[i], pc);
    }
}

TEST(ImageJPEG3, FreesMemoryOnNewLoad) {
    string f = JPEG_3X1_PX;
    Image image(3);
    image.loadImage(&prc, f.c_str());
    f = JPEG_INVALID;
    image.loadImage(&prc, f.c_str());
    ImageData data = image.getImageData();
    ASSERT_EQ(nullptr, data.data);
    ASSERT_EQ(0, data.metaData.pixelCount());
}

TEST(ImageJPEG3, SetPixels) {
    string f = JPEG_3X1_PX;
    Image image(3);
    image.loadImage(&prc, f.c_str());
    int data[3];
    int size = sizeof(data);
    memset(&data, 0, size);
    image.setPixels(data);

    ASSERT_EQ(BITMAP_COLOR(0xFFA42725), data[0]);
    ASSERT_EQ(BITMAP_COLOR(0xFFFFDAD8), data[1]);
    ASSERT_EQ(BITMAP_COLOR(0xFF000000), data[2]);
}

TEST(ImageJPEG3, Rotate180) {
    Image image(3);
    bytep_t *imageData = new bytep_t[12]{1, 2, 3, 4, 5, 6,
                                                     7, 8, 9, 10, 11, 12};

    //rotated position, color values must stay
    bytep_t imageDataExpected[] = {10, 11, 12, 7, 8, 9,
                                         4, 5, 6, 1, 2, 3};

    image.setRawData(imageData, 2, 2, 3);
    image.rotate180();

    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_1Slow) {
    Image image(3);
    bytep_t *imageData = new bytep_t[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};

    int imageDataExpected[] = {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9};

    image.setRawData(imageData, 3, 1, 3);
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_1Fast) {
    Image image(3);
    bytep_t *imageData = new bytep_t[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};

    int imageDataExpected[] = {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9};

    image.setRawData(imageData, 3, 1, 3);
    image.rotate90(true);
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_2Slow) {
    Image image(3);
    bytep_t *imageData = new bytep_t[6*3]{11, 12, 13, 21, 22, 23, 31, 32, 33,
                                                      41, 42, 43, 51, 52, 53, 61, 62, 63};

    bytep_t imageDataExpected[] = {41, 42, 43, 11, 12, 13,
                                         51, 52, 53, 21, 22, 23,
                                         61, 62, 63, 31, 32, 33};

    image.setRawData(imageData, 3, 2, 3);
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_2Fast) {
    Image image(3);
    bytep_t *imageData = new bytep_t[6*3]{11, 12, 13, 21, 22, 23, 31, 32, 33,
                                                      41, 42, 43, 51, 52, 53, 61, 62, 63};

    bytep_t imageDataExpected[] = {41, 42, 43, 11, 12, 13,
                                         51, 52, 53, 21, 22, 23,
                                         61, 62, 63, 31, 32, 33};

    image.setRawData(imageData, 3, 2, 3);
    image.rotate90(true);
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_3Slow) {
    Image image(3);
    bytep_t *imageData = new bytep_t[36]{11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42, 43,
                                                     51, 52, 53, 61, 62, 63, 71, 72, 73, 81, 82, 83,
                                                     91, 92, 93, 101, 102, 103, 111, 112, 113, 121, 122, 123};

    bytep_t imageDataExpected[] = {91, 92, 93, 51, 52, 53, 11, 12, 13,
                                         101, 102, 103, 61, 62, 63, 21, 22, 23,
                                         111, 112, 113, 71, 72, 73, 31, 32, 33,
                                         121, 122, 123, 81, 82, 83, 41, 42, 43};

    image.setRawData(imageData, 4, 3, 3);
    image.rotate90(false);

    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, Rotate90_3Fast) {
    Image image(3);
    bytep_t *imageData = new bytep_t[36]{11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42, 43,
                                                     51, 52, 53, 61, 62, 63, 71, 72, 73, 81, 82, 83,
                                                     91, 92, 93, 101, 102, 103, 111, 112, 113, 121, 122, 123};

    bytep_t imageDataExpected[] = {91, 92, 93, 51, 52, 53, 11, 12, 13,
                                         101, 102, 103, 61, 62, 63, 21, 22, 23,
                                         111, 112, 113, 71, 72, 73, 31, 32, 33,
                                         121, 122, 123, 81, 82, 83, 41, 42, 43};

    image.setRawData(imageData, 4, 3, 3);
    image.rotate90(true);
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}

TEST(ImageJPEG3, SaveImage) {
    Image image(3);
    string f = JPEG_SAMPLE_ASSET;
    image.loadImage(&prc, f.c_str());
    const char *path = "pokus3.jpg";

    image.saveImage(&prc, path);

    Image image2(3);
    image2.loadImage(&prc, path);
    const ImageMetaData metaData = image.getMetaData();
    ASSERT_EQ(image.getMetaData().imageWidth, metaData.imageWidth);
    ASSERT_EQ(image.getMetaData().imageHeight, metaData.imageHeight);
    remove(path);
}

TEST(ImageJPEG3, SetPixelsCrop) {
    Image image(3);
    bytep_t *imageData = new bytep_t[6 * 3]{0x11, 0x12, 0x13, 0x21, 0x22, 0x23, 0x31, 0x32, 0x33,
                                                        0x41, 0x42, 0x43, 0x51, 0x52, 0x53, 0x61, 0x62, 0x63};

    image.setRawData(imageData, 3, 2, 3);
    int target[4];

    //NO BITMAP_COLOR, because it's simple set!
    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 0, 0, 1, 1);
    ASSERT_EQ((0xFF111213), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 1, 1, 1, 1);
    ASSERT_EQ((0xFF515253), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 2, 0, 1, 1);
    ASSERT_EQ((0xFF313233), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 2, 1, 1, 1);
    ASSERT_EQ((0xFF616263), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 0, 0, 1, 1);
    ASSERT_EQ((0xFF111213), target[0]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 1, 0, 2, 2);
    ASSERT_EQ((0xFF212223), target[0]);
    ASSERT_EQ((0xFF313233), target[1]);
    ASSERT_EQ((0xFF515253), target[2]);
    ASSERT_EQ((0xFF616263), target[3]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 1, 0, 2, 1);
    ASSERT_EQ((0xFF212223), target[0]);
    ASSERT_EQ((0xFF313233), target[1]);

    memset(target, 0, sizeof(target));
    image.setPixels((int *) &target, 1, 0, 1, 2);
    ASSERT_EQ((0xFF212223), target[0]);
    ASSERT_EQ((0xFF515253), target[1]);
}
