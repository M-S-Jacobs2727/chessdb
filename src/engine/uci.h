#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <boost/process.hpp>

#include "internal/evaluation.h"
#include "internal/game/game.h"
#include "internal/game/move.h"
#include "internal/game/position.h"

namespace ChessGame
{
    namespace bp = boost::process;

    class UCI
    {
    public:
        UCI() = default;
    };
} // namespace ChessGame