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
        explicit UCI(std::string_view enginePath);
        void loadEngine(std::string_view enginePath);
        ~UCI();

        void poll();
        void parse(std::string_view line);
        void init();
        void setOptions();
        void newGame(const Position &position = Position());
        void applyMove(const Move &move);
        eval_t evaluation();
        Move bestMove();
        void stop();
        void start();

    private:
        std::optional<std::string_view> expect(std::string_view token, float withinSeconds = 5.0f);
        bool running();

    private:
        bp::ipstream m_engInput;
        bp::opstream m_engOutput;
        bp::child m_engine;
        std::vector<std::string> m_header;
        Position m_initialPosition;
        std::vector<Move> m_currentMoves;
        Move m_bestMove;
        std::string m_lastLine;
    };
} // namespace ChessGame