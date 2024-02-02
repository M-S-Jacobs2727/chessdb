#pragma once

#include <ostream>
#include <string>

#include "core/board.h"
#include "core/castling.h"
#include "core/state.h"

namespace ChessGame::FEN
{
    std::ostream &operator<<(std::ostream &os, const Board &board);
    std::ostream &operator<<(std::ostream &os, const State &state);
    constexpr std::string str(const Castling::Rights &rights);
    constexpr std::string str(const Board &board);
    constexpr std::string str(const State &state);
} // namespace ChessGame::FEN
