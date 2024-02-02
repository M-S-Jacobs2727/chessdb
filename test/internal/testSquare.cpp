#include "core/square.h"
#include <gtest/gtest.h>

using ChessGame::Square;

TEST(SquareTest, BasicAssertions)
{
    Square sq;
    EXPECT_EQ(sq.file, 0);
    EXPECT_EQ(sq.rank, 0);

    Square sq2{0, 0};
    EXPECT_EQ(sq, sq2);
}
