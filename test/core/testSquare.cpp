#include "core/square.h"
#include <gtest/gtest.h>

using JChess::Square;

TEST(SquareTest, BasicAssertions)
{
    Square sq;
    EXPECT_EQ(sq.file, 0);
    EXPECT_EQ(sq.rank, 0);

    Square sq2{0, 0};
    EXPECT_EQ(sq, sq2);

    Square sq3{"a3"};
    EXPECT_EQ(sq3.file, 0);
    EXPECT_EQ(sq3.rank, 2);
}

TEST(SquareTest, OffsetOperations)
{
    using JChess::Offset;
    Square sq{"b4"};
    Offset forward{0, 1};
    auto sq2 = sq + forward;
    EXPECT_EQ(sq2.file, 1);
    EXPECT_EQ(sq2.rank, 4);

    sq2 = sq - forward;
    EXPECT_EQ(sq2.file, 1);
    EXPECT_EQ(sq2.rank, 2);

    Offset off{2, 2};
    sq += off;
    EXPECT_EQ(sq.file, 3);
    EXPECT_EQ(sq.rank, 5);

    sq2 -= off;
    EXPECT_EQ(sq2.file, -1);
    EXPECT_EQ(sq2.rank, 0);

    auto offset = sq - sq2;
    EXPECT_EQ(offset.file, 4);
    EXPECT_EQ(offset.rank, 5);
}
