#pragma once

#include <boost/process.hpp>

#include "chessgame/evaluation.h"
#include "chessgame/game.h"
#include "chessgame/move.h"
#include "chessgame/position.h"

namespace ChessGame
{
    namespace bp = boost::process;

    class UCI
    {
    public:
        UCI() = default;
        explicit UCI(std::string_view enginePath);
        void loadEngine(std::string_view enginePath);
        ~UCI();

        void newGame(const Position &position = Position());
        void applyMove(const Move &move);
        eval_t evaluation();
        Move bestMove();
        void stop();
        void start();

    private:
        bp::ipstream m_engInput;
        bp::opstream m_engOutput;
        bp::child m_engine;
        bool m_running;
    };
} // namespace ChessGame