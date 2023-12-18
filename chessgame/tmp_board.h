#pragma once

#include <array>
#include <optional>
#include <string_view>
#include <unordered_set>

#include "piece.h"

namespace ChessGame
{
    enum class Direction // Oriented with black up, a1 in bottom left
    {
        UP = -8,
        DOWN = 8,
        LEFT = -1,
        RIGHT = 1,
        UPLEFT = -9,
        UPRIGHT = -7,
        DOWNLEFT = 7,
        DOWNRIGHT = 9
    };

    class Square
    {
    public:
        Square() = default;
        explicit Square(std::string_view);
        std::string_view str();
        uint8_t idx();

    private:
        uint8_t m_square = 0U;
    };

    class Board
    {
        using pieces_t = std::array<std::optional<Piece>, 64>;

    public:
        Board() = default;
        std::optional<Piece> get(std::string_view);
        std::optional<Piece> get(uint8_t);
        std::optional<Square> search(Square toSquare, Direction dir, Piece piece);
        Square findFromSquare(Square toSquare, Piece piece);

    public:
        static pieces_t startingPieces;

    private:
        pieces_t pieces = startingPieces;
    };
    Board::pieces_t Board::startingPieces = {
        Piece{Color::Black, PieceType::Rook},
        Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Bishop},
        Piece{Color::Black, PieceType::King},
        Piece{Color::Black, PieceType::Queen},
        Piece{Color::Black, PieceType::Bishop},
        Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn},
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Knight},
        Piece{Color::White, PieceType::Bishop},
        Piece{Color::White, PieceType::King},
        Piece{Color::White, PieceType::Queen},
        Piece{Color::White, PieceType::Bishop},
        Piece{Color::White, PieceType::Knight},
        Piece{Color::White, PieceType::Rook}};

} // namespace ChessGame
