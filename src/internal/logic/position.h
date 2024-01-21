#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "internal/logic/move.h"
#include "internal/logic/offset.h"
#include "internal/logic/piece.h"
#include "internal/logic/square.h"

#define INITFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace ChessGame
{
    class Position : public std::array<std::optional<Piece>, 64>
    {
    public:
        explicit Position(std::string_view fenString = INITFEN);
        inline std::optional<Piece> get(Square square) const;
        std::optional<Piece> put(Square square, Piece piece);
        std::optional<Piece> remove(Square square);
        constexpr Square kingSquare(Color color) const;
        constexpr std::string_view toFEN() const;
        constexpr std::vector<Square> getPath(Square fromSquare, Offset direction, bool includePiece) const;
        std::array<std::pair<Square, std::optional<Piece>>, 64> eachSquare() const;
        std::vector<std::pair<Square, std::optional<Piece>>> eachVisibleSquare(Square ref, Color color, PieceType pieceType) const;
    };
} // namespace ChessGame
