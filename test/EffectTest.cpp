//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../src/Effect.h"
#include "Assets.h"
#include "../src/Image.hpp"
#include "../src/PNGImageProcessor.hpp"
#include "../src/Errors.h"

TEST(PNGImage, Effect_grayScale) {
    string file = PNG_4x4RGB_ASSET;
    Image image(4);
    PNGImageProcessor prc;
    IOResult ior = image.loadImage(&prc, file.c_str());
    ASSERT_EQ(NO_ERR, ior.result);

    const ImageData data = image.getImageData();
    EffectFunction f = Effect::get(GRAYSCALE);
    f(data.data, data.metaData.imageWidth, data.metaData.imageHeight, 4, nullptr);
    image.saveImage(&prc, "grayscale.png");
}
