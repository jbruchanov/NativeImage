//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/PNGImageProcessor.hpp"
#include "../src/Errors.h"
#include "Assets.h"
#include "../src/Debug.h"

#define PX(a, b, c) (0xFF << 24 | a << 16 | b << 8 | c)

bytep_t *getTestData() {
    //released when image gets released
    return new bytep_t[48]
            {0x65, 0x66, 0x67, 0x6F, 0x70, 0x71, 0x79, 0x7A, 0x7B, 0x83, 0x84, 0x85,
             0x8D, 0x8E, 0x8F, 0x97, 0x98, 0x99, 0xA1, 0xA2, 0xA3, 0xAB, 0xAC, 0xAD,
             0xB5, 0xB6, 0xB7, 0xBF, 0xC0, 0xC1, 0xC9, 0xCA, 0xCB, 0xD3, 0xD4, 0xD5,
             0xDD, 0xDE, 0xDF, 0xE7, 0xE8, 0xE9, 0xF1, 0xF2, 0xF3, 0xFB, 0xFC, 0xFD};
}

TEST(Image, SetPixelsScale) {
    Image image(3);
    image.setRawData(getTestData(), 4, 4, 3);

    int data[1];
    image.setPixelsScale((int *) &data, 1, 1, 1, 1, 1, 1);

    ASSERT_EQ(PX(0x97, 0x98, 0x99), data[0]);
}

TEST(Image, SetPixelsScale2) {
    Image image(3);
    image.setRawData(getTestData(), 4, 4, 3);

    int data[4];
    image.setPixelsScale((int *) &data, 2, 2, 1, 1, 2, 2);

    ASSERT_EQ(PX(0x97, 0x98, 0x99), data[0]);
    ASSERT_EQ(PX(0xA1, 0xA2, 0xA3), data[1]);
    ASSERT_EQ(PX(0xBF, 0xC0, 0xC1), data[2]);
    ASSERT_EQ(PX(0xC9, 0xCA, 0xCB), data[3]);
}

TEST(Image, SetPixelsScale3) {
    Image image(3);
    image.setRawData(getTestData(), 4, 4, 3);

    int data[4];
    image.setPixelsScale((int *) &data, 2, 2, 0, 0, 4, 4);

    ASSERT_EQ(PX(0x65, 0x66, 0x67), data[0]);
    ASSERT_EQ(PX(0x79, 0x7A, 0x7B), data[1]);
    ASSERT_EQ(PX(0xB5, 0xB6, 0xB7), data[2]);
    ASSERT_EQ(PX(0xC9, 0xCA, 0xCB), data[3]);
}

TEST(Image, SetPixelsScale4) {
    Image image(3);
    image.setRawData(getTestData(), 4, 4, 3);

    int data[4];
    image.setPixelsScale((int *) &data, 2, 2, 1, 1, 3, 3);

    ASSERT_EQ(PX(0x97, 0x98, 0x99), data[0]);
    ASSERT_EQ(PX(0xA1, 0xA2, 0xA3), data[1]);
    ASSERT_EQ(PX(0xBF, 0xC0, 0xC1), data[2]);
    ASSERT_EQ(PX(0xC9, 0xCA, 0xCB), data[3]);
}