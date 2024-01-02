#pragma once

#include <istream>
#include <ostream>
#include <string_view>

#include "internal/game/game.h"
#include "internal/game/move.h"

namespace ChessGame
{
    void readPGNHeader(std::istream &input, Game &game);
    void readPGNMoves(std::istream &input, Game &game);
    Game readPGN(std::istream &input);
} // namespace ChessGame
