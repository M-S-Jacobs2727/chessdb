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
