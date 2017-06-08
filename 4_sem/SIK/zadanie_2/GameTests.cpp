#include "gtest/gtest.h"

#include "Position.h"

TEST(GameTest, PositionTest) {
    Position p = Position::generate();
    EXPECT_LE(p.getDirection(), 360);
}