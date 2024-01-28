#include "internal/logic/square.h"
#include <gtest/gtest.h>

using ChessGame::Square;

TEST(SquareTest, BasicAssertions)
{
    Square sq;
    ASSERT_EQ(sq.file, 0);
    ASSERT_EQ(sq.rank, 0);
    ASSERT_TRUE(sq.valid());

    ASSERT_EQ(sq.idx(), 56);
    Square sq2{0, 0};
    ASSERT_EQ(sq, sq2);
}

TEST(SquareTest, Constructors)
{
    using namespace std::string_view_literals;

    Square sq{1, 1};
    Square sq2("b2"sv);
    ASSERT_EQ(sq, sq2);

    Square sq3 = Square::fromIdx(56);
    ASSERT_EQ(sq, sq3);
}

TEST(SquareTest, Strings)
{
    using namespace std::string_view_literals;

    Square sq{1, 1};
    ASSERT_EQ(sq.str(), "b2"sv);

    Square sq2("h8"sv);
    ASSERT_EQ(sq2.str(), "h8"sv);
}

TEST(SquareTest, Validity)
{
    Square sq{8, 1};
    ASSERT_FALSE(sq.valid());

    Square sq2{100, 100};
    ASSERT_FALSE(sq.valid());

    Square sq3{7, 7};
    ASSERT_TRUE(sq.valid());
}