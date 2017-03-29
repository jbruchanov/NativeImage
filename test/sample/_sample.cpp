//
// Created by jbruchanov on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../../src/SimpleClass.h"
#include "../../json11/json11.hpp"

TEST(Sample, SimpleAssertTrue) {
    bool value = true;
    ASSERT_TRUE(value);
}

TEST(Sample, ProjectFolderAccess) {
    SimpleClass c;
    int result = c.add(1, 2);
    ASSERT_EQ(3, result);
}


TEST(Sample, Json) {
    json11::Json result = json11::Json::object {
            { "a", 1 },
            { "b", 2 }};
    ASSERT_EQ("{\"a\": 1, \"b\": 2}", result.dump());
}
