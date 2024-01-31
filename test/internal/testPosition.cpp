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
            EXPECT_EQ(static_cast<bool>(p1), static_cast<bool>(p2));
            if (p1)
                EXPECT_EQ(p1.value(), p2.value());
        }

    auto p = pos.get({0, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wr);
    p = pos.get({1, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wn);
    p = pos.get({2, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wb);
    p = pos.get({3, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wq);
    p = pos.get({4, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wk);
    p = pos.get({5, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wb);
    p = pos.get({6, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wn);
    p = pos.get({7, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wr);

    p = pos.get({0, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), br);
    p = pos.get({1, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bn);
    p = pos.get({2, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bb);
    p = pos.get({3, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bq);
    p = pos.get({4, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bk);
    p = pos.get({5, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bb);
    p = pos.get({6, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), bn);
    p = pos.get({7, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), br);

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 1});
        ASSERT_TRUE(p);
        EXPECT_EQ(p.value(), wp);
    }

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 6});
        ASSERT_TRUE(p);
        EXPECT_EQ(p.value(), bp);
    }

    for (size_t j = 2; j < 6; ++j)
        for (size_t i = 0; i < 8; ++i)
            EXPECT_FALSE(pos.get({i, j}));
}

TEST(PositionTest, PutRemove)
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
    Position pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    EXPECT_FALSE(pos.remove({0, 3}));

    auto p = pos.remove({0, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wr);
    EXPECT_FALSE(pos.get({0, 0}));

    p = pos.put({0, 1}, wq);
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wp);

    p = pos.get({0, 1});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.value(), wq);
}

TEST(PositionTest, KingSquare)
{
    Position pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    ChessGame::Square ws = pos.kingSquare(ChessGame::Color::White),
                      bs = pos.kingSquare(ChessGame::Color::Black);
    EXPECT_EQ(ws.file, 4);
    EXPECT_EQ(ws.rank, 0);
    EXPECT_EQ(bs.file, 4);
    EXPECT_EQ(bs.rank, 7);

    Position pos2{"8/8/8/k6K/8/8/8/8"};
    ws = pos2.kingSquare(ChessGame::Color::White);
    bs = pos2.kingSquare(ChessGame::Color::Black);
    EXPECT_EQ(ws.file, 7);
    EXPECT_EQ(ws.rank, 4);
    EXPECT_EQ(bs.file, 0);
    EXPECT_EQ(bs.rank, 4);
}

TEST(PositionTest, GetPath)
{
    using ChessGame::Offset, ChessGame::Square;
    Position pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    Square start{3, 1};
    Offset forward{0, 1};
    auto path = pos.getPath(start, forward, true);

    ASSERT_EQ(path.size(), 5);
    for (size_t i = 2; i < 7; ++i)
    {
        EXPECT_EQ(path[i - 2].file, 3);
        EXPECT_EQ(path[i - 2].rank, i);
    }

    path = pos.getPath(start, forward, false);
    ASSERT_EQ(path.size(), 4);
    for (size_t i = 2; i < 6; ++i)
    {
        EXPECT_EQ(path[i - 2].file, 3);
        EXPECT_EQ(path[i - 2].rank, i);
    }

    Offset diag{-1, 1};
    path = pos.getPath(start, diag, true);
    auto path2 = pos.getPath(start, diag, false);
    ASSERT_EQ(path.size(), 3);
    ASSERT_EQ(path2.size(), 3);
    for (size_t i = 2; i < 5; ++i)
    {
        EXPECT_EQ(path[i - 2].file, 4 - i);
        EXPECT_EQ(path[i - 2].rank, i);
        EXPECT_EQ(path2[i - 2].file, 4 - i);
        EXPECT_EQ(path2[i - 2].rank, i);
    }
}