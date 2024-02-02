#include "engine/uci.h"

#include "uci.h"
#include <chrono>
#include <sstream>

namespace ChessGame
{
    UCI::UCI(std::string_view enginePath)
        : m_engine(bp::child(enginePath,
                             (bp::std_err & bp::std_out) > m_engInput,
                             bp::std_in < m_engOutput)),
          m_engInput(), m_engOutput(), m_initialPosition(initFEN)
    {
        using namespace std::chrono_literals;

        m_readDelay = 10ms;

        std::string line;
        m_header.reserve(24);

        m_engOutput << "uci" << std::endl;
        while (running() &&
               std::getline(m_engInput, line) &&
               line != "uciok")
            m_header.push_back(line);
    }

    UCI::~UCI()
    {
        using namespace std::chrono_literals;
        stop();
        m_engOutput << "quit" << std::endl;

        if (!m_engine.wait_for(1s))
            m_engine.terminate();
    }

    void UCI::nextPosition(const Board &pos)
    {
        m_engOutput << "board " << pos.toFEN() << std::endl;
    }

    void UCI::poll()
    {
        auto ret = expect("", 0.2f);
        m_lastLine = ret.value_or(m_lastLine);

        parse(m_lastLine);
    }

    void UCI::parse(std::string_view line)
    {
        std::istringstream is{line.data()};
        std::string word;
        is >> word;
        if (word != "info")
            throw std::runtime_error("invalid line from engine");

        while (is && !word.empty())
        {
            is >> word;
            if (word == "string")
                return;

            if (word == "score")
            {
                is >> word;
                if (word == "mate")
                    ;
                else if (word == "cp")
                    ;
                else
                    throw std::runtime_error("Invalid score keyword in engine line");
                is >> word;
                int score = std::stoi(word);
            }
            else if (word == "pv")
            {
                std::getline(is, word);
            }
        }
    }

    void UCI::newGame(const Board &board)
    {
        m_initialPosition = board;
        m_engOutput << "ucinewgame" << std::endl;

        checkReady();

        m_engOutput << "board " << board.toFEN() << std::endl;
    }

    void UCI::applyMove(const Move &move)
    {
        m_currentMoves.push_back(move);
        m_engOutput << "board " << m_initialPosition.toFEN() << " moves";
        for (const auto &move : m_currentMoves)
            m_engOutput << ' ' << move.from.str();
        m_engOutput << std::endl;
    }

    void UCI::start()
    {
        m_engOutput << "go infinite" << std::endl;
        m_lastLine.clear();
    }

    void UCI::checkReady()
    {
        m_engOutput << "isready" << std::endl;
        auto line = readline();
        if (!line || line.value() != "readyok")
            throw std::runtime_error("Engine failure");
    }

    std::optional<std::string_view> UCI::expect(std::string_view token, float withinMilliseconds)
    {
        if (withinMilliseconds <= 0.0f)
            throw std::runtime_error("Invalid number of seconds");

        std::future fut = std::async(std::launch::async, [&]()
                                     {std::string line;
                                      std::getline(this->m_engInput, line);
                                      return line; });

        auto res = fut.wait_for(std::chrono::duration<float>(withinMilliseconds));
        if (res == std::future_status::ready)
        {
            auto str = fut.get();
            if (token.empty() || token == str)
                return std::make_optional(std::string_view(str));
        }
        return std::nullopt;
    }

    void UCI::stop()
    {
        m_engOutput << "stop" << std::endl;
    }

    inline bool UCI::running()
    {
        return m_engine.running();
    }

    void UCI::kill()
    {
        m_engine.terminate();
    }

    std::optional<std::string> UCI::readline()
    {
        std::future fut = std::async(std::launch::async, [&]()
                                     {std::string line;
                                      std::getline(this->m_engInput, line);
                                      return line; });
        auto status = fut.wait_for(m_readDelay);
        if (status != std::future_status::ready)
            return std::nullopt;

        return fut.get();
    }
} // namespace ChessGame
