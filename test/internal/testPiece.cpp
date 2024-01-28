#include "internal/logic/piece.h"
#include <gtest/gtest.h>

using ChessGame::Piece, ChessGame::PieceType, ChessGame::Color;

TEST(PieceTest, BasicAssertions)
{
    Piece piece{Color::White, PieceType::Knight};
    EXPECT_EQ(piece.color, Color::White);
    EXPECT_EQ(piece.type, PieceType::Knight);
}

TEST(PieceTest, OppositeColor)
{
    using ChessGame::oppositeColor;
    EXPECT_EQ(oppositeColor(Color::White), Color::Black);
    EXPECT_EQ(oppositeColor(Color::Black), Color::White);
}

TEST(PieceTest, ReadPGN)
{
    using ChessGame::readPGNPieceType;
    EXPECT_EQ(readPGNPieceType("exd4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("a4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("b4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("c4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("d4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("e4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("f4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("g4"), PieceType::Pawn);
    EXPECT_EQ(readPGNPieceType("h1=R"), PieceType::Pawn);

    EXPECT_EQ(readPGNPieceType("Nh4+"), PieceType::Knight);
    EXPECT_EQ(readPGNPieceType("Bh4#"), PieceType::Bishop);
    EXPECT_EQ(readPGNPieceType("Rhh4"), PieceType::Rook);
    EXPECT_EQ(readPGNPieceType("Qh4"), PieceType::Queen);
    EXPECT_EQ(readPGNPieceType("Kh4"), PieceType::King);
    EXPECT_EQ(readPGNPieceType("O-O"), PieceType::King);
    EXPECT_EQ(readPGNPieceType("O-O-O"), PieceType::King);
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
