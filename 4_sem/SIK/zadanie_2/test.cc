#include "gtest/gtest.h"

#include "SerializerTests.cpp"
#include "GameTests.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
