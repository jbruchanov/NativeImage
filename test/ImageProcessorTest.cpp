//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/ImageProcessor.hpp"


TEST(PNGImage, ImageRemoveAlphaChannel) {
    bytep_t *imageData = new bytep_t[24]{0x11, 0x12, 0x13, 0x14, 0x21, 0x22, 0x23, 0x24, 0x31, 0x32, 0x33, 0x34,
                                                     0x41, 0x42, 0x43, 0x44, 0x51, 0x52, 0x53, 0x54, 0x61, 0x62, 0x63, 0x64};

    bytep_t imageDataExpected[] = {0x12, 0x13, 0x14, 0x22, 0x23, 0x24, 0x32, 0x33, 0x34,
                                         0x42, 0x43, 0x44, 0x52, 0x53, 0x54, 0x62, 0x63, 0x64};

    bytep_t* newData = ImageProcessor::removeAlpha(imageData, 3, 2, 4);
    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], newData[i]);
    }
}

TEST(PNGImage, ImageRemoveAlphaChannel2) {
    bytep_t *imageData = new bytep_t[24]
            {0x80, 0xFF, 0x00, 0x00,
             0x80, 0x00, 0xFF, 0x00,
             0x80, 0x00, 0x00, 0xFF};



    bytep_t imageDataExpected[] = {0xFF, 0x00, 0x00,
                                         0x00, 0xFF, 0x00,
                                         0x00, 0x00, 0xFF};

    bytep_t* newData = ImageProcessor::removeAlpha(imageData, 3, 1, 4);
    for (int i = 0, n = sizeof(imageData) / sizeof(bytep_t); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], newData[i]);
    }
}
