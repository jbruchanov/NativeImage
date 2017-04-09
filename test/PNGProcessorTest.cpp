//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/PNGImageProcessor.hpp"


TEST(PNGImage, AddAlphaChannel) {
    unsigned char *imageData = new unsigned char[4]{0x11, 0x12, 0x13, 0x00};
    PNGImageProcessor::addAlpha(imageData, 1, 1);
    ASSERT_EQ(0xFF111213, imageData[0] << 24 | imageData[1] << 16 | imageData[2] << 8 | imageData[3]);
}

TEST(PNGImage, AddAlphaChannel2) {
    unsigned char *imageData = new unsigned char[24]
            {0x11, 0x12, 0x13, 0x21, 0x22, 0x23, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00,
             0x41, 0x42, 0x43, 0x51, 0x52, 0x53, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00};

    unsigned char expectedData[24] = {0xFF, 0x11, 0x12, 0x13, 0xFF, 0x21, 0x22, 0x23, 0xFF, 0x31, 0x32, 0x33,
                                      0xFF, 0x41, 0x42, 0x43, 0xFF, 0x51, 0x52, 0x53, 0xFF, 0x61, 0x62, 0x63};

    PNGImageProcessor::addAlpha(imageData, 3, 2);
    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(expectedData[i], imageData[i]);
    }
}