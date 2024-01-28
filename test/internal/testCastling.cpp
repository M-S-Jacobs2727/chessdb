#include "internal/logic/castling.h"
#include <gtest/gtest.h>

namespace cstl = ChessGame::Castling;

TEST(CastlingTest, BasicAssertions)
{
    using ChessGame::Color, cstl::Side;

    cstl::Rights rights;
    EXPECT_TRUE(rights.get(Color::White, Side::QUEEN));
    EXPECT_TRUE(rights.get(Color::White, Side::KING));
    EXPECT_TRUE(rights.get(Color::Black, Side::QUEEN));
    EXPECT_TRUE(rights.get(Color::Black, Side::KING));
}

TEST(CastlingTest, Squares)
{
    using ChessGame::Square, ChessGame::Color, ChessGame::Castling::Side, ChessGame::Castling::Castle;
    Square sq, sq2;

    sq = cstl::kingToSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("c1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::White, Side::KING});
    sq2 = Square("g1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("c8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::kingToSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("g8");
    EXPECT_EQ(sq, sq2);

    sq = cstl::rookToSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("d1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::White, Side::KING});
    sq2 = Square("f1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("d8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookToSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("f8");
    EXPECT_EQ(sq, sq2);

    sq = cstl::rookFromSquare(Castle{Color::White, Side::QUEEN});
    sq2 = Square("a1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::White, Side::KING});
    sq2 = Square("h1");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::Black, Side::QUEEN});
    sq2 = Square("a8");
    EXPECT_EQ(sq, sq2);
    sq = cstl::rookFromSquare(Castle{Color::Black, Side::KING});
    sq2 = Square("h8");
    EXPECT_EQ(sq, sq2);
}

TEST(CastlingTest, FENConstructor)
{
    using ChessGame::Color, ChessGame::Castling::Side;

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
    using ChessGame::Color, ChessGame::Castling::Side, ChessGame::Castling::Castle;

    Castle wk{Color::White, Side::KING},
        wq{Color::White, Side::QUEEN},
        bk{Color::Black, Side::KING},
        bq{Color::Black, Side::QUEEN};

    cstl::Rights rights;
    rights.remove(wk);
    EXPECT_FALSE(rights.get(wk.color, wk.side));
    EXPECT_TRUE(rights.get(wq.color, wq.side));
    EXPECT_TRUE(rights.get(bk.color, bk.side));
    EXPECT_TRUE(rights.get(bq.color, bq.side));

    rights.remove(Color::Black);
    EXPECT_FALSE(rights.get(wk.color, wk.side));
    EXPECT_TRUE(rights.get(wq.color, wq.side));
    EXPECT_FALSE(rights.get(bk.color, bk.side));
    EXPECT_FALSE(rights.get(bq.color, bq.side));

    rights.remove(wq);
    EXPECT_FALSE(rights.get(wk.color, wk.side));
    EXPECT_FALSE(rights.get(wq.color, wq.side));
    EXPECT_FALSE(rights.get(bk.color, bk.side));
    EXPECT_FALSE(rights.get(bq.color, bq.side));
}