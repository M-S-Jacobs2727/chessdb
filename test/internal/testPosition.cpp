#include "internal/logic/position.h"
#include <gtest/gtest.h>

using ChessGame::Position;

TEST(PositionTest, BasicAssertions)
{
    using ChessGame::Color, ChessGame::Piece, ChessGame::PieceType;
    Piece wp{Color::White, PieceType::Pawn},
        wn{Color::White, PieceType::Knight},
        wb{Color::White, PieceType::Bishop},
        wr{Color::White, PieceType::Rook},
        wq{Color::White, PieceType::Queen},
        wk{Color::White, PieceType::King},
        bp{Color::Black, PieceType::Pawn},
        bn{Color::Black, PieceType::Knight},
        bb{Color::Black, PieceType::Bishop},
        br{Color::Black, PieceType::Rook},
        bq{Color::Black, PieceType::Queen},
        bk{Color::Black, PieceType::King};

    Position pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    Position pos2{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    for (size_t i = 0; i < 8; ++i)
        for (size_t j = 0; j < 8; ++j)
        {
            auto p1 = pos.get({i, j}), p2 = pos2.get({i, j});
            ASSERT_EQ(p1, p2);
            if (p1)
                ASSERT_EQ(p1.value(), p2.value());
        }

    auto p = pos.get({0, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wr);
    p = pos.get({1, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wn);
    p = pos.get({2, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wb);
    p = pos.get({3, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wq);
    p = pos.get({4, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wk);
    p = pos.get({5, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wb);
    p = pos.get({6, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wn);
    p = pos.get({7, 0});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), wr);

    p = pos.get({0, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), br);
    p = pos.get({1, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bn);
    p = pos.get({2, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bb);
    p = pos.get({3, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bq);
    p = pos.get({4, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bk);
    p = pos.get({5, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bb);
    p = pos.get({6, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), bn);
    p = pos.get({7, 7});
    ASSERT_TRUE(p);
    ASSERT_EQ(p.value(), br);

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 1});
        ASSERT_TRUE(p);
        ASSERT_EQ(p.value(), wp);
    }

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 6});
        ASSERT_TRUE(p);
        ASSERT_EQ(p.value(), bp);
    }

    for (size_t j = 2; j < 6; ++j)
        for (size_t i = 0; i < 8; ++i)
            ASSERT_FALSE(pos.get({i, j}));
}
