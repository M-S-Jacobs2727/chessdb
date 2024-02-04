#pragma once

#include <istream>
#include <ostream>
#include <string_view>

#include "internal/game/game.h"
#include "internal/game/move.h"

namespace JChess
{
    Game readPGN(std::istream &input);

    void readPGNHeader(std::istream &input, Game &game);
    void readPGNMoves(std::istream &input, Game &game);
    Move readPGNMove(std::string_view pgnMove,
                     const Board &pos,
                     const std::optional<Square> &enPassant,
                     Color turn);
} // namespace ChessGame
