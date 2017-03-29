//
// Created by scurab on 29/03/17.
//

#include <googletest/include/gtest/gtest.h>
#include "../../src/SimpleClass.h"

TEST(Sample, SimpleAssertTrue) {
    bool value = true;
    ASSERT_TRUE(value);
}

TEST(Sample, ProjectFolderAccess) {
    SimpleClass c;
    int result = c.add(1, 2);
    ASSERT_EQ(3, result);
}
