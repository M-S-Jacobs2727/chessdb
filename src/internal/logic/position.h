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

namespace ChessGame
{
    constexpr std::string_view initFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    class Position
    {
    public:
        explicit Position(std::string_view fenString);
        std::optional<Piece> get(Square square) const;
        std::optional<Piece> put(Square square, Piece piece);
        std::optional<Piece> remove(Square square);
        Square kingSquare(Color color) const;
        constexpr std::string_view toFEN() const;
        std::vector<Square> getPath(Square fromSquare, Offset direction, bool includePiece) const;
        std::array<std::pair<Square, std::optional<Piece>>, 64> eachSquare() const;
        std::vector<std::pair<Square, std::optional<Piece>>> eachVisibleSquare(Square ref, Color color, PieceType pieceType) const;

    private:
        std::array<std::optional<Piece>, 64> m_arr;
    };
} // namespace ChessGame
