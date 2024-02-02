#include "formats/fen.h"

#include <array>

namespace ChessGame::FEN
{
    std::array<char, 12> fenPieces = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    constexpr inline size_t pieceToIdx(Piece piece)
    {
        return static_cast<size_t>(piece.color) * 6 + static_cast<size_t>(piece.type);
    }
    std::ostream &operator<<(std::ostream &os, const Board &board)
    {
        os << str(board);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const State &state)
    {
        os << str(state);
        return os;
    }

    constexpr std::string str(const Castling::Rights &rights)
    {
        std::string castlestr;
        castlestr.reserve(5);
        auto r = rights.get();
        const std::string test{"KQkq"};
        for (size_t i = 0; i < 4; ++i)
            if (r[i])
                castlestr += test[i];
        if (castlestr.empty())
            castlestr += '-';
        return castlestr;
    }

    constexpr std::string str(const State &state)
    {
        auto fenstr = str(state.board);
        fenstr += ' ';
        fenstr += (state.turn == Color::White) ? 'w' : 'b';
        fenstr += ' ';
        fenstr += str(state.castleRights);
        fenstr += ' ';
        if (state.enPassant)
        {
            fenstr += static_cast<char>('a' + state.enPassant.value().file);
            fenstr += static_cast<char>('1' + state.enPassant.value().rank);
        }
        else
            fenstr += '-';
        fenstr += ' ';
        fenstr += std::to_string(state.halfTurnCounter);
        fenstr += ' ';
        fenstr += std::to_string(state.fullTurnCounter);
    }

    constexpr std::string str(const Board &board)
    {
        std::string fenstr;
        fenstr.reserve(64); // 64 squares max + 7 /'s

        const auto &occupants = board.eachOccupant();

        size_t i = 0;
        for (size_t row = 0; row < 8; ++row)
        {
            if (row != 0)
                fenstr += '/';

            size_t empties = 0;
            for (size_t col = 0; col < 8; ++col, ++i)
            {
                if (occupants[i])
                {
                    if (empties)
                        fenstr += static_cast<char>('0' + empties);

                    fenstr += fenPieces[pieceToIdx(occupants[i].piece())];
                }
                else
                    ++empties;
            }
            if (empties)
                fenstr += static_cast<char>('0' + empties);
        }
    }
} // namespace ChessGame::FEN
