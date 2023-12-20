#pragma once

#include <istream>
#include <ostream>
#include <string_view>

#include "chessgame/game.h"
#include "chessgame/move.h"

namespace ChessGame
{
    void readPGNHeader(std::istream &input, Game &game);
    void readPGNMoves(std::istream &input, Game &game);
    Game readPGN(std::istream &input);
} // namespace ChessGame
