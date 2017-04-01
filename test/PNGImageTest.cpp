//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/PNGImage.hpp"
#include "../src/Errors.h"
#include "TestHelper.h"
#include "jerror.h"

#define PNG_1X1_PX TEST_ASSET("assets/red.png")
//#define JPEG_3X1_PX TEST_ASSET("assets/3pixels.jpg")
//#define JPEG_INVALID TEST_ASSET("assets/invalid.jpg")
//#define JPEG_SAMPLE_ASSET TEST_ASSET("assets/test.jpg")


TEST(PNGImage, LoadingImage) {
    string file = PNG_1X1_PX;
    PNGImage image;
    int v = image.loadImage(file.c_str());
    ASSERT_EQ(NO_ERR, v);
}

