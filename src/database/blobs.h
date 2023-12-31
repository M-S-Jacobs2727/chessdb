#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <pqxx/pqxx>

#include "internal/game/move.h"
#include "internal/game/position.h"

namespace ChessGame
{
    using blob = std::basic_string<std::byte>;

    blob movesToBlob(const std::vector<Move> &moves);
    std::vector<Move> blobToMoves(const blob &movesBlob);

    blob positionToBlob(const Position &position);
    Position blobToPosition(const blob &posBlob);
} // namespace ChessGame