#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/square.h"

#define INITFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace ChessGame
{
    class Position : public std::array<Piece, 64>
    {
    public:
        explicit Position(std::string_view fenString = INITFEN);
        Piece get(Square square) const;
        Piece put(Square square, Piece piece);
        std::string_view toFEN() const;

    private:
        uint8_t idx(Square square) const;
        Square idxToSquare(uint8_t idx) const;
    };
} // namespace ChessGame
