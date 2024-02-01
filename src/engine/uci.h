#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <boost/process.hpp>

#include "internal/annotation/evaluation.h"
#include "internal/logic/move.h"
#include "internal/logic/position.h"

namespace ChessGame
{
    namespace bp = boost::process;

    class UCI
    {
    public:
        struct Score
        {
            enum class Units
            {
                CP,
                MATE
            };
            int score;
            Units units;
            bool upperbound = false;
            bool lowerbound = false;
        };
        struct EngineInfo
        {
            int searchdepth;
            int seldepth;
            int msSearched;
            int nodesSearched;
            std::vector<Move> bestLine;
            int multipv;
            Score score;
            Move searchingMove;
            int searchingMoveNum;
        };

    public:
        UCI() = delete;
        explicit UCI(std::string_view enginePath);
        ~UCI();

        /// @brief Set UCI options for this instance (not implemented).
        void setOptions();

        /// @brief Set up engine for a new game starting at `position`.
        /// @param position An initial starting Position
        void newGame(const Position &position);

        /// @brief Update the engine's board position with the given move.
        /// @param move
        void applyMove(const Move &move);

        /// @brief Apply a new position to the engine's state.
        /// Note: Do not use for a new game. Call `UCI::newGame(pos)` instead.
        /// @param pos The new position to evaluate.
        void nextPosition(const Position &pos);

        void poll();
        eval_t evaluation();
        Move bestMove();
        void stop();
        void start();

    private:
        void parse(std::string_view line);

        /// @brief Send an 'isready' message, wait for a 'readyok' message, throw if not received.
        void checkReady();
        std::optional<std::string_view> expect(std::string_view token, float withinSeconds = 5.0f);
        bool running();
        void kill();

        /// @brief Read the next line from the engine, if it exists.
        /// @return An optional string containing the line or nothing.
        std::optional<std::string> readline();

    private:
        bp::ipstream m_engInput;
        bp::opstream m_engOutput;
        bp::child m_engine;

        std::vector<std::string> m_header;
        Position m_initialPosition;
        std::vector<Move> m_currentMoves;
        Move m_bestMove;
        std::string m_lastLine;

        std::chrono::duration<float> m_readDelay;
    };
} // namespace ChessGame