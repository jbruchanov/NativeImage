//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/PNGImageProcessor.hpp"
#include "../src/Errors.h"
#include "Assets.h"


TEST(PNGImage, ImageRemoveAlphaChannel) {
    Image image(4);
    unsigned char *imageData = new unsigned char[24]{0x11, 0x12, 0x13, 0x14, 0x21, 0x22, 0x23, 0x24, 0x31, 0x32, 0x33, 0x34,
                                                     0x41, 0x42, 0x43, 0x44, 0x51, 0x52, 0x53, 0x54, 0x61, 0x62, 0x63, 0x64};

    unsigned char imageDataExpected[] = {0x12, 0x13, 0x14, 0x22, 0x23, 0x24, 0x32, 0x33, 0x34,
                                         0x42, 0x43, 0x44, 0x52, 0x53, 0x54, 0x62, 0x63, 0x64};

    image.setRawData(imageData, 3, 2, 4);
    ASSERT_EQ(RGBA, image.getComponentsPerPixel());
    image.removeAlpha();
    ASSERT_EQ(RGB, image.getComponentsPerPixel());
    imageData = image.getImageData().data;
    for (int i = 0, n = sizeof(imageData) / sizeof(unsigned char); i < n; i++) {
        ASSERT_EQ(imageDataExpected[i], imageData[i]);
    }
}