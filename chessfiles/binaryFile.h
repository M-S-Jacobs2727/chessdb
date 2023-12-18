#pragma once

#include <istream>
#include <ostream>

#include "board.h"
#include "clockTime.h"
#include "evaluation.h"
#include "game.h"

namespace ChessGame
{
    void writeBinary(std::ostream &output, const Game &game);
    void writeBinaryHeader(std::ostream &output, const Game &game);
    void writeBinaryMove(std::ostream &output, const Move &move);
    void writeBinaryEvaluation(std::ostream &output, const eval_t &eval);
    void writeBinaryClock(std::ostream &output, const uint16_t secondsRemaining);
    void writeBinaryState(std::ostream &output, const Board &board);

    Game readBinary(std::istream &input);
    void readBinaryHeader(std::istream &input, Game &game);
    void readBinaryMove(std::istream &input, Move &move);
    void readBinaryEvaluation(std::istream &input, Evaluation &eval);
    void readBinaryClock(std::istream &input, ClockTime &clock);
    void readBinaryState(std::istream &input, Board &board);
} // namespace ChessGame
