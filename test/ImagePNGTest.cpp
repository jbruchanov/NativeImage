//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/PNGImageProcessor.hpp"
#include "../src/Errors.h"
#include "Assets.h"
#include "../src/Debug.h"


TEST(PNGImage, LoadingImage3_RGB) {
    string file = PNG_4x4RGB_ASSET;
    Image image(3);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
    unsigned char *data = ior.data;

    ASSERT_EQ(BITMAP_COLOR(0x6D8920), data[0] << 16 | data[1] << 8 | data[2]);
    ASSERT_EQ(BITMAP_COLOR(0xE66A51), data[15] << 16 | data[16] << 8 | data[17]);
    ASSERT_EQ(BITMAP_COLOR(0x603243), data[30] << 16 | data[31] << 8 | data[32]);
    ASSERT_EQ(BITMAP_COLOR(0xF236C2), data[45] << 16 | data[46] << 8 | data[47]);
}

TEST(PNGImage, LoadingImage4_RGBA) {
    string file = PNG_3X1RGBA_PX;
    Image image(4);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
    unsigned char *data = ior.data;

    ASSERT_EQ(BITMAP_COLOR(0x80FF0000),  data[0] << 24 |  data[1] << 16 | data[2] << 8 | data[3]);
    ASSERT_EQ(BITMAP_COLOR(0x8000FF00),  data[4] << 24 |  data[5] << 16 | data[6] << 8 | data[7]);
    ASSERT_EQ(BITMAP_COLOR(0x800000FF),  data[8] << 24 |  data[9] << 16 | data[10] << 8 | data[11]);
}

TEST(PNGImage, LoadingImage3_RGBA) {
    string file = PNG_3X1RGBA_PX;
    Image image(3);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
    unsigned char *data = ior.data;

    ASSERT_EQ(BITMAP_COLOR(0xFF0000),  data[0] << 16 | data[1] << 8 | data[2]);
    ASSERT_EQ(BITMAP_COLOR(0x00FF00),  data[3] << 16 | data[4] << 8 | data[5]);
    ASSERT_EQ(BITMAP_COLOR(0x0000FF),  data[6] << 16 | data[7] << 8 | data[8]);
}

TEST(PNGImage, LoadingImage4_RGB) {
    string file = PNG_3X1RGB_PX;
    Image image(4);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
    unsigned char *data = ior.data;
    ASSERT_EQ(BITMAP_COLOR(0xFFFF0000), data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
    ASSERT_EQ(BITMAP_COLOR(0xFF00FF00), data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7]);
    ASSERT_EQ(BITMAP_COLOR(0xFF0000FF), data[8] << 24 | data[9] << 16 | data[10] << 8 | data[11]);
}

TEST(PNGImage, LoadingImage3Large_RGB) {
    string file = PNG_4x4RGB_ASSET;
    Image image(4);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);
    unsigned char *data = ior.data;

    int offset = 0;
    ASSERT_EQ(BITMAP_COLOR(0xFF6D8920), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset += 4;
    ASSERT_EQ(BITMAP_COLOR(0xFFDFF00E), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset += 4;
    ASSERT_EQ(BITMAP_COLOR(0xFFB6F4D3), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset += 4;
    ASSERT_EQ(BITMAP_COLOR(0xFF2532F5), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);

    offset += 4;
    ASSERT_EQ(BITMAP_COLOR(0xFFA8C45D), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);

    offset = 20;//Pixel@[1,1]
    ASSERT_EQ(BITMAP_COLOR(0xFFE66A51), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset = 40;//Pixel@[2,2]
    ASSERT_EQ(BITMAP_COLOR(0xFF603243), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset = 60;//Pixel@[3,3]
    ASSERT_EQ(BITMAP_COLOR(0xFFF236C2), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
}