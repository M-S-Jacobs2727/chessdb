#include "internal/logic/square.h"
#include <gtest/gtest.h>

using ChessGame::Square;

TEST(SquareTest, BasicAssertions)
{
    Square sq;
    EXPECT_EQ(sq.file, 0);
    EXPECT_EQ(sq.rank, 0);
    EXPECT_TRUE(sq.valid());

    EXPECT_EQ(sq.idx(), 56);
    Square sq2{0, 0};
    EXPECT_EQ(sq, sq2);
}

TEST(SquareTest, Constructors)
{
    using namespace std::string_view_literals;

    Square sq{1, 1};
    Square sq2("b2"sv);
    EXPECT_EQ(sq, sq2);

    Square sq3 = Square::fromIdx(49);
    EXPECT_EQ(sq, sq3);
}

TEST(SquareTest, Strings)
{
    using namespace std::string_view_literals;

    Square sq{1, 1};
    EXPECT_EQ(sq.str(), "b2"sv);

    Square sq2("h8"sv);
    EXPECT_EQ(sq2.str(), "h8"sv);
}

TEST(SquareTest, Validity)
{
    Square sq{8, 1};
    EXPECT_FALSE(sq.valid());

    sq.file = 100;
    sq.rank = 100;
    EXPECT_FALSE(sq.valid());

    sq.file = 7;
    sq.rank = 7;
    EXPECT_TRUE(sq.valid());
}