#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "core/castling.h"
#include "core/piece.h"
#include "core/square.h"

namespace JChess
{
    struct Move
    {
        Piece piece;
        Square from;
        Square to;

        bool enPassant = false;
        std::optional<Piece> capture = std::nullopt;
        std::optional<Castling::Side> castle = std::nullopt;
        std::optional<Piece> promotion = std::nullopt;

        bool operator==(const Move &other) const
        {
            return piece == other.piece &&
                   from == other.from &&
                   to == other.to;
        }
    };
} // namespace JChess
