#include "internal/logic/offset.h"
#include <gtest/gtest.h>

using ChessGame::Offset;

TEST(OffsetTest, BasicAssertions)
{
    Offset offset;
    ASSERT_EQ(offset.file, 0);
    ASSERT_EQ(offset.rank, 0);

    Offset offset2{-1, 8};
    ASSERT_EQ(offset2.file, -1);
    ASSERT_EQ(offset2.rank, 8);
}

TEST(OffsetTest, Operations)
{
    Offset offset, offset2{0, 0}, offset3;
    ASSERT_EQ(offset, offset2);

    offset2.file = 1;
    offset3 = offset + offset2;
    ASSERT_EQ(offset3, offset2);
    ASSERT_EQ(offset, offset2 - offset3);

    offset3 = -offset2;
    ASSERT_EQ(offset3.file, -offset2.file);

    offset3 = offset2 * 2;
    ASSERT_EQ(offset3.file, 2);
    ASSERT_EQ(offset3.rank, 0);

    offset = offset3 / (-2);
    ASSERT_EQ(offset.file, -1);
    ASSERT_EQ(offset.rank, 0);

    offset2.rank = 1;
    offset2.file = 1;

    offset += offset2;
    ASSERT_EQ(offset.file, 0);
    ASSERT_EQ(offset.rank, 1);

    offset -= offset3;
    ASSERT_EQ(offset.file, -2);
    ASSERT_EQ(offset.rank, 1);

    offset *= -3;
    ASSERT_EQ(offset.file, 6);
    ASSERT_EQ(offset.rank, -3);

    offset /= 2;
    ASSERT_EQ(offset.file, 3);
    ASSERT_EQ(offset.rank, -1);
}

TEST(OffsetTest, LatDiag)
{
    Offset offset{1, 1};
    ASSERT_TRUE(offset.isDiagonal());

    offset.rank = 0;
    ASSERT_TRUE(offset.isLateral());
}

TEST(OffsetTest, Norm)
{
    Offset offset{4, -4};
    ASSERT_TRUE(offset.norm());
    ASSERT_EQ(offset.file, 1);
    ASSERT_EQ(offset.rank, -1);

    Offset offset2{5, 0};
    ASSERT_TRUE(offset.norm());
    ASSERT_EQ(offset.file, 1);
    ASSERT_EQ(offset.rank, 0);

    Offset offset3{5, 1};
    ASSERT_FALSE(offset.norm());
    ASSERT_EQ(offset.file, 5);
    ASSERT_EQ(offset.rank, 1);
}

TEST(OffsetTest, SquareOperator)
{
    using ChessGame::Square;

    Square sq("a1");
    Offset offset{1, 2};

    auto maybeSq = offset(sq);
    ASSERT_TRUE(maybeSq);
    Square sq2 = maybeSq.value();

    ASSERT_EQ(sq2.file, 1);
    ASSERT_EQ(sq2.rank, 2);

    sq.file = 7;
    maybeSq = offset(sq);
    ASSERT_FALSE(maybeSq);

    sq.file = 0;
    sq.rank = 6;
    maybeSq = offset(sq);
    ASSERT_FALSE(maybeSq);
}

TEST(OffsetTest, Difference)
{
    using ChessGame::Square;

    Square sq("a1"), sq2("g1");
    Offset offset = ChessGame::difference(sq, sq2);
    ASSERT_EQ(offset.file, 6);
    ASSERT_EQ(offset.rank, 0);

    Offset offset2 = ChessGame::difference(sq2, sq);
    ASSERT_EQ(offset, -offset2);
}

TEST(OffsetTest, BackAndForth)
{
    using ChessGame::Color;

    Offset offset = ChessGame::forward(Color::White),
           offset2 = ChessGame::backward(Color::Black);
    ASSERT_EQ(offset, offset2);
    ASSERT_EQ(offset.file, 0);
    ASSERT_EQ(offset.rank, 1);

    offset = ChessGame::forward(Color::Black);
    offset2 = ChessGame::backward(Color::White);
    ASSERT_EQ(offset, offset2);
    ASSERT_EQ(offset.file, 0);
    ASSERT_EQ(offset.rank, -1);
}