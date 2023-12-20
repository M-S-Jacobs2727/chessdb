#pragma once

#include <vector>

#include "chessgame/game.h"
#include "chessgame/move.h"

namespace ChessGame
{
    std::string_view assembleMovesBlob(const std::vector<Move> &moves);
    std::string_view insertGameCommand(const Game &game);
} // namespace ChessGame
