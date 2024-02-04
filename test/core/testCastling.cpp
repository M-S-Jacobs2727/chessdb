#include "core/castling.h"
#include <gtest/gtest.h>

namespace cstl = JChess::Castling;

TEST(CastlingTest, BasicAssertions)
{
    using JChess::Color, cstl::Side;

    cstl::Rights rights;
    EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
    EXPECT_TRUE(rights.get(Color::White, Side::KING));
    EXPECT_TRUE(rights.get(Color::Black, Side::QUEEN));
    EXPECT_TRUE(rights.get(Color::Black, Side::KING));
}

TEST(CastlingTest, FENConstructor)
{
    using JChess::Color, JChess::Castling::Side;

    {
        cstl::Rights rights("KQkq");
        EXPECT_TRUE(rights.get(Color::White, Side::KING));
        EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
        EXPECT_TRUE(rights.get(Color::Black, Side::KING));
        EXPECT_TRUE(rights.get(Color::Black, Side::QUEEN));
    }
    {
        cstl::Rights rights("KQ");
        EXPECT_TRUE(rights.get(Color::White, Side::KING));
        EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
        EXPECT_FALSE(rights.get(Color::Black, Side::KING));
        EXPECT_FALSE(rights.get(Color::Black, Side::QUEEN));
    }
    {
        cstl::Rights rights("Kk");
        EXPECT_TRUE(rights.get(Color::White, Side::KING));
        EXPECT_FALSE(rights.get(Color::White, Side::QUEEN));
        EXPECT_TRUE(rights.get(Color::Black, Side::KING));
        EXPECT_FALSE(rights.get(Color::Black, Side::QUEEN));
    }
    {
        cstl::Rights rights("q");
        EXPECT_FALSE(rights.get(Color::White, Side::KING));
        EXPECT_FALSE(rights.get(Color::White, Side::QUEEN));
        EXPECT_FALSE(rights.get(Color::Black, Side::KING));
        EXPECT_TRUE(rights.get(Color::Black, Side::QUEEN));
    }
    {
        cstl::Rights rights("-");
        EXPECT_FALSE(rights.get(Color::White, Side::KING));
        EXPECT_FALSE(rights.get(Color::White, Side::QUEEN));
        EXPECT_FALSE(rights.get(Color::Black, Side::KING));
        EXPECT_FALSE(rights.get(Color::Black, Side::QUEEN));
    }
}

TEST(CastlingTest, Remove)
{
    using JChess::Color, JChess::Castling::Side;

    cstl::Rights rights;
    rights.remove(Color::White, Side::KING);
    EXPECT_FALSE(rights.get(Color::White, Side::KING));
    EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
    EXPECT_TRUE(rights.get(Color::Black, Side::KING));
    EXPECT_TRUE(rights.get(Color::Black, Side::QUEEN));

    rights.remove(Color::Black);
    EXPECT_FALSE(rights.get(Color::White, Side::KING));
    EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
    EXPECT_FALSE(rights.get(Color::Black, Side::KING));
    EXPECT_FALSE(rights.get(Color::Black, Side::QUEEN));

    rights.remove(Color::White, Side::QUEEN);
    EXPECT_FALSE(rights.get(Color::White, Side::KING));
    EXPECT_FALSE(rights.get(Color::White, Side::QUEEN));
    EXPECT_FALSE(rights.get(Color::Black, Side::KING));
    EXPECT_FALSE(rights.get(Color::Black, Side::QUEEN));
}