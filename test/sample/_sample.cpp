//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../../src/SimpleClass.h"
#include "../../json11/json11.hpp"
#include "../../src/ImageProcessor.h"
#include "../../src/JpegImageProcessor.h"
#include "../../src/Image.hpp"
#include "../Assets.h"

using namespace std;

TEST(Sample, SimpleAssertTrue) {
    bool value = true;
    ASSERT_TRUE(value);
}

TEST(Sample, ProjectFolderAccess) {
    SimpleClass c;
    int result = c.add(1, 2);
    ASSERT_EQ(3, result);
}


TEST(Sample, ToJson) {
    json11::Json result = json11::Json::object {
            { "a", 1 },
            { "b", 2 }};
    ASSERT_EQ("{\"a\": 1, \"b\": 2}", result.dump());
}

TEST(Sample, FromJson) {
    const std::string jsonString = "{\"a\": 1, \"b\": 2}";
    std::string err;
    json11::Json json = json11::Json::parse(jsonString, err);
    int value = json["a"].int_value();
    bool ismissing = json["c"].is_null();
    ASSERT_EQ(1, value);
    ASSERT_TRUE(ismissing);
}


ImageProcessor *getProcessor() {
    return new JpegImageProcessor();
}

TEST(Sample, TestUniquePtr) {
    try {
        unique_ptr<ImageProcessor> ptr(getProcessor());
        throw 1;
    } catch (...) {

    }
}

TEST(ImageJPEG3, RotationSpeed) {
    Image image(3);
    string f = JPEG_SAMPLE_ASSET;
    JpegImageProcessor prc;
    image.loadImage(&prc, f.c_str());

    for(int x = 0;x<2;x++) {
        long sum = 0;
        int repeats = 10;
        for (int i = 0; i < repeats; i++) {
            clock_t start = clock();
            image.rotate90(x == 1);
            clock_t end = clock();
            sum += (end - start);
        }
        printf("\nRotate90 avg:%ld fast:%s\n", sum / repeats, (x == 1) ? "true" : "false");
    }
}