#include "core/piece.h"
#include <gtest/gtest.h>

using JChess::Piece, JChess::PieceType, JChess::Color;

TEST(PieceTest, BasicAssertions)
{
    Piece piece{Color::White, PieceType::Knight};
    EXPECT_EQ(piece.color, Color::White);
    EXPECT_EQ(piece.type, PieceType::Knight);
}

TEST(PieceTest, OppositeColor)
{
    using JChess::oppositeColor;
    EXPECT_EQ(oppositeColor(Color::White), Color::Black);
    EXPECT_EQ(oppositeColor(Color::Black), Color::White);
}

TEST(PieceTest, Operators)
{
    Piece piece{Color::White, PieceType::Pawn};
    EXPECT_EQ(piece, Piece(Color::White, PieceType::Pawn));

    EXPECT_NE(piece, Piece(Color::White, PieceType::Knight));
    EXPECT_NE(piece, Piece(Color::White, PieceType::Bishop));
    EXPECT_NE(piece, Piece(Color::White, PieceType::Rook));
    EXPECT_NE(piece, Piece(Color::White, PieceType::Queen));
    EXPECT_NE(piece, Piece(Color::White, PieceType::King));

    EXPECT_NE(piece, Piece(Color::Black, PieceType::Pawn));
    EXPECT_NE(piece, Piece(Color::Black, PieceType::Knight));
    EXPECT_NE(piece, Piece(Color::Black, PieceType::Bishop));
    EXPECT_NE(piece, Piece(Color::Black, PieceType::Rook));
    EXPECT_NE(piece, Piece(Color::Black, PieceType::Queen));
    EXPECT_NE(piece, Piece(Color::Black, PieceType::King));
}
