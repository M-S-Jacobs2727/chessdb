#include "internal/logic/castling.h"
#include <gtest/gtest.h>

namespace cstl = ChessGame::Castling;

TEST(CastlingTest, BasicAssertions)
{
    using ChessGame::Color, cstl::Side;

    cstl::Rights rights;
    ASSERT_TRUE(rights.get(Color::White, Side::QUEEN));
    ASSERT_TRUE(rights.get(Color::White, Side::KING));
    ASSERT_TRUE(rights.get(Color::Black, Side::QUEEN));
    ASSERT_TRUE(rights.get(Color::Black, Side::KING));
}

TEST(CastlingTest, Squares)
{
    using ChessGame::Square, ChessGame::Color, ChessGame::Castling::Side, ChessGame::Castling::Castle;
    Square sq, sq2;

    sq = cstl::kingToSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("c1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::White, Side::KING});
    sq2 = Square("g1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("c8");
    ASSERT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("g8");
    ASSERT_EQ(sq, sq2);

    sq = cstl::rookToSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("d1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::White, Side::KING});
    sq2 = Square("f1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("d8");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("f8");
    ASSERT_EQ(sq, sq2);

    sq = cstl::rookFromSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("a1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::White, Side::KING});
    sq2 = Square("h1");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("a8");
    ASSERT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("h8");
    ASSERT_EQ(sq, sq2);
}
