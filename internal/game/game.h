#pragma once

#include <optional>
#include <string>
#include <vector>

#include "clockTime.h"
#include "datetime.h"
#include "evaluation.h"
#include "move.h"
#include "result.h"
#include "state.h"
#include "timeControl.h"

namespace ChessGame
{
    struct Game
    {
        std::string whiteUsername;
        std::string blackUsername;
        Datetime datetime;
        uint16_t whiteELO = 0;
        uint16_t blackELO = 0;
        std::string ECOCode;
        TimeControl timeControl;
        GameResult result;

        std::vector<Move> moves;
        std::optional<std::vector<ClockTime>> clocks = std::nullopt;
        std::optional<std::vector<Evaluation>> evaluations = std::nullopt;
        std::vector<State> states;
    };
}