#include "core/board.h"
#include <gtest/gtest.h>

using JChess::Board;

TEST(PositionTest, BasicAssertions)
{
    using JChess::Color, JChess::Piece, JChess::PieceType;
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

    Board pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    Board pos2{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    for (size_t i = 0; i < 8; ++i)
        for (size_t j = 0; j < 8; ++j)
        {
            auto p1 = pos.get({i, j}), p2 = pos2.get({i, j});
            EXPECT_EQ(static_cast<bool>(p1), static_cast<bool>(p2));
            if (p1)
                EXPECT_EQ(p1.piece(), p2.piece());
        }

    auto p = pos.get({0, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wr);
    p = pos.get({1, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wn);
    p = pos.get({2, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wb);
    p = pos.get({3, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wq);
    p = pos.get({4, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wk);
    p = pos.get({5, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wb);
    p = pos.get({6, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wn);
    p = pos.get({7, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wr);

    p = pos.get({0, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), br);
    p = pos.get({1, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bn);
    p = pos.get({2, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bb);
    p = pos.get({3, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bq);
    p = pos.get({4, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bk);
    p = pos.get({5, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bb);
    p = pos.get({6, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), bn);
    p = pos.get({7, 7});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), br);

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 1});
        ASSERT_TRUE(p);
        EXPECT_EQ(p.piece(), wp);
    }

    for (size_t i = 0; i < 8; ++i)
    {
        p = pos.get({i, 6});
        ASSERT_TRUE(p);
        EXPECT_EQ(p.piece(), bp);
    }

    for (size_t j = 2; j < 6; ++j)
        for (size_t i = 0; i < 8; ++i)
            EXPECT_FALSE(pos.get({i, j}));
}

TEST(PositionTest, PutRemove)
{
    using JChess::Color, JChess::Piece, JChess::PieceType;

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
    Board pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    EXPECT_FALSE(pos.remove({0, 3}));

    auto p = pos.remove({0, 0});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wr);
    EXPECT_FALSE(pos.get({0, 0}));

    p = pos.put({0, 1}, wq);
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wp);

    p = pos.get({0, 1});
    ASSERT_TRUE(p);
    EXPECT_EQ(p.piece(), wq);
}

TEST(PositionTest, KingSquare)
{
    Board pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    JChess::Square ws = pos.kingSquare(JChess::Color::White),
                      bs = pos.kingSquare(JChess::Color::Black);
    EXPECT_EQ(ws.file, 4);
    EXPECT_EQ(ws.rank, 0);
    EXPECT_EQ(bs.file, 4);
    EXPECT_EQ(bs.rank, 7);

    Board pos2{"8/8/8/k6K/8/8/8/8"};
    ws = pos2.kingSquare(JChess::Color::White);
    bs = pos2.kingSquare(JChess::Color::Black);
    EXPECT_EQ(ws.file, 7);
    EXPECT_EQ(ws.rank, 4);
    EXPECT_EQ(bs.file, 0);
    EXPECT_EQ(bs.rank, 4);
}

TEST(PositionTest, GetPath)
{
    using JChess::Offset, JChess::Square;
    Board pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
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