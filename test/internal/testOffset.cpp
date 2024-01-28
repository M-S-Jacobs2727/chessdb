#include "internal/logic/offset.h"
#include <gtest/gtest.h>

using ChessGame::Offset;

TEST(OffsetTest, BasicAssertions)
{
    Offset offset;
    EXPECT_EQ(offset.file, 0);
    EXPECT_EQ(offset.rank, 0);

    Offset offset2{-1, 8};
    EXPECT_EQ(offset2.file, -1);
    EXPECT_EQ(offset2.rank, 8);
}

TEST(OffsetTest, Operations)
{
    Offset offset, offset2{0, 0}, offset3;
    EXPECT_EQ(offset, offset2);

    offset2.file = 1;
    offset3 = offset + offset2;
    EXPECT_EQ(offset3, offset2);
    EXPECT_EQ(offset, offset2 - offset3);

    offset3 = -offset2;
    EXPECT_EQ(offset3.file, -offset2.file);

    offset3 = offset2 * 2;
    EXPECT_EQ(offset3.file, 2);
    EXPECT_EQ(offset3.rank, 0);

    offset = offset3 / (-2);
    EXPECT_EQ(offset.file, -1);
    EXPECT_EQ(offset.rank, 0);

    offset2.rank = 1;
    offset2.file = 1;

    offset += offset2;
    EXPECT_EQ(offset.file, 0);
    EXPECT_EQ(offset.rank, 1);

    offset -= offset3;
    EXPECT_EQ(offset.file, -2);
    EXPECT_EQ(offset.rank, 1);

    offset *= -3;
    EXPECT_EQ(offset.file, 6);
    EXPECT_EQ(offset.rank, -3);

    offset /= 2;
    EXPECT_EQ(offset.file, 3);
    EXPECT_EQ(offset.rank, -1);
}

TEST(OffsetTest, LatDiag)
{
    Offset offset{1, 1};
    EXPECT_TRUE(offset.isDiagonal());

    offset.rank = 0;
    EXPECT_TRUE(offset.isLateral());
}

TEST(OffsetTest, Norm)
{
    Offset offset{4, -4};
    EXPECT_TRUE(offset.norm());
    EXPECT_EQ(offset.file, 1);
    EXPECT_EQ(offset.rank, -1);

    Offset offset2{5, 0};
    EXPECT_TRUE(offset.norm());
    EXPECT_EQ(offset.file, 1);
    EXPECT_EQ(offset.rank, 0);

    Offset offset3{5, 1};
    EXPECT_FALSE(offset.norm());
    EXPECT_EQ(offset.file, 5);
    EXPECT_EQ(offset.rank, 1);
}

TEST(OffsetTest, SquareOperator)
{
    using ChessGame::Square;

    Square sq("a1");
    Offset offset{1, 2};

    auto maybeSq = offset(sq);
    ASSERT_TRUE(maybeSq);
    Square sq2 = maybeSq.value();

    EXPECT_EQ(sq2.file, 1);
    EXPECT_EQ(sq2.rank, 2);

    sq.file = 7;
    maybeSq = offset(sq);
    EXPECT_FALSE(maybeSq);

    sq.file = 0;
    sq.rank = 6;
    maybeSq = offset(sq);
    EXPECT_FALSE(maybeSq);
}

TEST(OffsetTest, Difference)
{
    using ChessGame::Square;

    Square sq("a1"), sq2("g1");
    Offset offset = ChessGame::difference(sq, sq2);
    EXPECT_EQ(offset.file, 6);
    EXPECT_EQ(offset.rank, 0);

    Offset offset2 = ChessGame::difference(sq2, sq);
    EXPECT_EQ(offset, -offset2);
}

TEST(OffsetTest, BackAndForth)
{
    using ChessGame::Color;

    Offset offset = ChessGame::forward(Color::White),
           offset2 = ChessGame::backward(Color::Black);
    EXPECT_EQ(offset, offset2);
    EXPECT_EQ(offset.file, 0);
    EXPECT_EQ(offset.rank, 1);

    offset = ChessGame::forward(Color::Black);
    offset2 = ChessGame::backward(Color::White);
    EXPECT_EQ(offset, offset2);
    EXPECT_EQ(offset.file, 0);
    EXPECT_EQ(offset.rank, -1);
}