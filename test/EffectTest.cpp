//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/Effect.h"
#include "Assets.h"
#include "../src/Image.hpp"
#include "../src/PNGImageProcessor.hpp"
#include "../src/Errors.h"
#include "../src/JpegImageProcessor.h"

TEST(Effect, Effect_grayScale) {
    string file = PNG_4x4RGB_ASSET;
    Image image(4);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);

    const ImageData data = image.getImageData();
    EffectFunction f = Effect::get(EFF_GRAYSCALE);
    f(data.data, data.metaData.imageWidth, data.metaData.imageHeight, 4, nullptr);
    image.saveImage(&prc, "grayscale.png");
}

TEST(Effect, Effect_crop) {
    string file = PNG_4x4RGB_ASSET;
    Image image(RGB);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    
    EffectFunction f = Effect::get(EFF_CROP);
    Json args = Json::object {
            {"offsetX", 2},
            {"offsetY", 2},
            {"width",   2},
            {"height",  2}};

    
    int result = image.applyFilter(f, args);
    ASSERT_EQ(NO_ERR, result);
    ASSERT_EQ(2, image.getMetaData().imageWidth);
    ASSERT_EQ(2, image.getMetaData().imageHeight);

    const ImageData im = image.getImageData();
    bytep_t *data = im.data;

    int offset = 0;
    ASSERT_EQ(BITMAP_COLOR(0x603243), data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
    offset += RGB;
    ASSERT_EQ(BITMAP_COLOR(0xEB954A), data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
    offset += RGB;
    ASSERT_EQ(BITMAP_COLOR(0xC27C96), data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
    offset += RGB;
    ASSERT_EQ(BITMAP_COLOR(0xF236C2), data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
}

TEST(Effect, Effect_crop2) {
    string file = PNG_4x4RGB_ASSET;
    Image image(RGBA);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());

    EffectFunction f = Effect::get(EFF_CROP);
    Json args = Json::object {
            { "offsetX", 0 },
            { "offsetY", 3 },
            { "width", 2 },
            { "height", 1 }};


    int result = image.applyFilter(f, args);
    ASSERT_EQ(NO_ERR, result);
    ASSERT_EQ(2, image.getMetaData().imageWidth);
    ASSERT_EQ(1, image.getMetaData().imageHeight);

    const ImageData im = image.getImageData();
    bytep_t *data = im.data;

    int offset = 0;
    ASSERT_EQ(BITMAP_COLOR(0xFFFCB892), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
    offset += RGBA;
    ASSERT_EQ(BITMAP_COLOR(0xFF692000), data[offset + 0] << 24 | data[offset + 1] << 16 | data[offset + 2] << 8 | data[offset + 3]);
}

TEST(Effect, Effect_brightness) {
    string file = PNG_4x4RGB_ASSET;
    Image image(RGBA);
    PNGImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_BRIGHTNESS);
    Json args = Json::object {{"brightness", 50}};
    int result = image.applyFilter(f, args);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "brightness.png");
}

TEST(Effect, Effect_contrast) {
    string file = JPEG_WALLPAPER;
    Image image(RGBA);
    JpegImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_CONTRAST);
    Json args = Json::object {{"contrast", 64}};
    int result = image.applyFilter(f, args);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "contrast.jpg");
}

TEST(Effect, Effect_gamma) {
    string file = JPEG_WALLPAPER;
    Image image(RGBA);
    JpegImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_GAMMA);
    Json args = Json::object {{"gamma", 2.25}};
    int result = image.applyFilter(f, args);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "gamma.jpg");
}

TEST(Effect, Effect_inverse) {
    string file = JPEG_WALLPAPER;
    Image image(RGBA);
    JpegImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_INVERSE);
    Json empty = Json::object {};
    int result = image.applyFilter(f, empty);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "inverse.jpg");
}

TEST(Effect, Effect_flipv) {
    string file = JPEG_WALLPAPER;
    Image image(RGBA);
    JpegImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_FLIP_VERTICAL);
    Json empty = Json::object {};
    int result = image.applyFilter(f, empty);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "flipv.jpg");
}

TEST(Effect, Effect_fliph) {
    string file = JPEG_WALLPAPER;
    Image image(RGBA);
    JpegImageProcessor prc;
    image.loadImage(&prc, file.c_str());
    EffectFunction f = Effect::get(EFF_FLIP_HORIZONTAL);
    Json empty = Json::object {};
    int result = image.applyFilter(f, empty);
    ASSERT_EQ(NO_ERR, result);
    image.saveImage(&prc, "fliph.jpg");
}
