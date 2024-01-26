#include "internal/logic/piece.h"
#include <gtest/gtest.h>

using ChessGame::Piece, ChessGame::PieceType, ChessGame::Color;

TEST(PieceTest, BasicAssertions)
{
    Piece piece{Color::White, PieceType::Knight};
    ASSERT_EQ(piece.color, Color::White);
    ASSERT_EQ(piece.type, PieceType::Knight);
}

TEST(PieceTest, OppositeColor)
{
    using ChessGame::oppositeColor;
    ASSERT_EQ(oppositeColor(Color::White), Color::Black);
    ASSERT_EQ(oppositeColor(Color::Black), Color::White);
}

TEST(PieceTest, ReadPGN)
{
    using ChessGame::readPGNPieceType;
    ASSERT_EQ(readPGNPieceType("exd4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("a4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("b4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("c4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("d4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("e4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("f4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("g4"), PieceType::Pawn);
    ASSERT_EQ(readPGNPieceType("h1=R"), PieceType::Pawn);

    ASSERT_EQ(readPGNPieceType("Nh4+"), PieceType::Knight);
    ASSERT_EQ(readPGNPieceType("Bh4#"), PieceType::Bishop);
    ASSERT_EQ(readPGNPieceType("Rhh4"), PieceType::Rook);
    ASSERT_EQ(readPGNPieceType("Qh4"), PieceType::Queen);
    ASSERT_EQ(readPGNPieceType("Kh4"), PieceType::King);
    ASSERT_EQ(readPGNPieceType("O-O"), PieceType::King);
    ASSERT_EQ(readPGNPieceType("O-O-O"), PieceType::King);
}

TEST(PieceTest, Operators)
{
    Piece piece{Color::White, PieceType::Pawn};
    ASSERT_EQ(piece, Piece(Color::White, PieceType::Pawn));

    ASSERT_NE(piece, Piece(Color::White, PieceType::Knight));
    ASSERT_NE(piece, Piece(Color::White, PieceType::Bishop));
    ASSERT_NE(piece, Piece(Color::White, PieceType::Rook));
    ASSERT_NE(piece, Piece(Color::White, PieceType::Queen));
    ASSERT_NE(piece, Piece(Color::White, PieceType::King));

    ASSERT_NE(piece, Piece(Color::Black, PieceType::Pawn));
    ASSERT_NE(piece, Piece(Color::Black, PieceType::Knight));
    ASSERT_NE(piece, Piece(Color::Black, PieceType::Bishop));
    ASSERT_NE(piece, Piece(Color::Black, PieceType::Rook));
    ASSERT_NE(piece, Piece(Color::Black, PieceType::Queen));
    ASSERT_NE(piece, Piece(Color::Black, PieceType::King));
}
