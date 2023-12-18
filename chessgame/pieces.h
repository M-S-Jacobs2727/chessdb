#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

#include "move.h"
#include "piece.h"
#include "square.h"

#define INITFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace ChessGame
{
    class Pieces : public std::array<Piece, 64>
    {
    public:
        explicit Pieces(std::string_view fenString = INITFEN);
        Piece get(Square square);
        Piece put(Square square, Piece piece);

    private:
        uint8_t idx(Square square);
        Square idxToSquare(uint8_t idx);
    };
} // namespace ChessGame
