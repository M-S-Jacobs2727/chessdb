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
