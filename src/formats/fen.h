#pragma once

#include <stdexcept>
#include <string_view>
#include <variant>

#include "core/piece.h"

namespace ChessGame::FEN
{
    constexpr std::string_view startpos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    constexpr std::string_view startstate{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    constexpr std::string_view startcastle{"KQkq"};

    constexpr int charToCastleIdx(char c)
    {
        int idx = -1;
        switch (c)
        {
        case 'K':
            idx = 0;
            break;
        case 'Q':
            idx = 1;
            break;
        case 'k':
            idx = 2;
            break;
        case 'q':
            idx = 3;
            break;

        default:
            throw std::runtime_error("Invalid character in castle section of FEN string.");
            break;
        }

        return idx;
    }

    std::unordered_map<char, size_t> m_fenMap = {
        {'K', 0},
        {'Q', 1},
        {'k', 2},
        {'q', 3},
    };
    constexpr std::variant<Piece, int> charToPiece(char c)
    {
        std::variant<Piece, int> occ;
        switch (c)
        {
        case 'P':
            occ = Piece{Color::White, PieceType::Pawn};
            break;
        case 'N':
            occ = Piece{Color::White, PieceType::Knight};
            break;
        case 'B':
            occ = Piece{Color::White, PieceType::Bishop};
            break;
        case 'R':
            occ = Piece{Color::White, PieceType::Rook};
            break;
        case 'Q':
            occ = Piece{Color::White, PieceType::Queen};
            break;
        case 'K':
            occ = Piece{Color::White, PieceType::King};
            break;
        case 'p':
            occ = Piece{Color::Black, PieceType::Pawn};
            break;
        case 'n':
            occ = Piece{Color::Black, PieceType::Knight};
            break;
        case 'b':
            occ = Piece{Color::Black, PieceType::Bishop};
            break;
        case 'r':
            occ = Piece{Color::Black, PieceType::Rook};
            break;
        case 'q':
            occ = Piece{Color::Black, PieceType::Queen};
            break;
        case 'k':
            occ = Piece{Color::Black, PieceType::King};
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            occ = static_cast<int>(c - '0');
            break;

        case '/':
            occ = 0;
            break;

        default:
            throw std::runtime_error("Invalid character");
            break;
        }

        return occ;
    }
    constexpr std::array<char, 12> pieceChars = {'p', 'n', 'b', 'r', 'q', 'k', 'P', 'N', 'B', 'R', 'Q', 'K'};
    constexpr inline char pieceToChar(Piece piece)
    {
        return pieceChars[static_cast<int>(piece.color) * 6 + static_cast<int>(piece.type)];
    }
} // namespace ChessGame::FEN
