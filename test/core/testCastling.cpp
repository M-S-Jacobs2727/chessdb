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

TEST(CastlingTest, Squares)
{
    using JChess::Square, JChess::Color, JChess::Castling::Side;
    Square sq, sq2;

    sq = cstl::kingToSquare(Color::White, Side::QUEEN);
    sq2 = Square("c1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Color::White, Side::KING);
    sq2 = Square("g1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Color::Black, Side::QUEEN);
    sq2 = Square("c8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Color::Black, Side::KING);
    sq2 = Square("g8");
    EXPECT_EQ(sq, sq2);

    sq = cstl::rookToSquare(Color::White, Side::QUEEN);
    sq2 = Square("d1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Color::White, Side::KING);
    sq2 = Square("f1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Color::Black, Side::QUEEN);
    sq2 = Square("d8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Color::Black, Side::KING);
    sq2 = Square("f8");
    EXPECT_EQ(sq, sq2);

    sq = cstl::rookFromSquare(Color::White, Side::QUEEN);
    sq2 = Square("a1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Color::White, Side::KING);
    sq2 = Square("h1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Color::Black, Side::QUEEN);
    sq2 = Square("a8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Color::Black, Side::KING);
    sq2 = Square("h8");
    EXPECT_EQ(sq, sq2);
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