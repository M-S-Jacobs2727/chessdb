#include "pieces.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ChessGame
{
    Position::Position(std::string_view fenString)
    {
        std::unordered_map<char, Piece> pieceMap = {
            {'p', Piece{Color::Black, PieceType::Pawn}},
            {'P', Piece{Color::White, PieceType::Pawn}},
            {'n', Piece{Color::Black, PieceType::Knight}},
            {'N', Piece{Color::White, PieceType::Knight}},
            {'b', Piece{Color::Black, PieceType::Bishop}},
            {'B', Piece{Color::White, PieceType::Bishop}},
            {'r', Piece{Color::Black, PieceType::Rook}},
            {'R', Piece{Color::White, PieceType::Rook}},
            {'q', Piece{Color::Black, PieceType::Queen}},
            {'Q', Piece{Color::White, PieceType::Queen}},
            {'k', Piece{Color::Black, PieceType::King}},
            {'K', Piece{Color::White, PieceType::King}},
        };

        uint8_t row = 0, col = 0, numWhiteKings = 0, numBlackKings = 0;
        for (const auto &c : fenString)
        {
            switch (c)
            {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                for (int i = 0; i < c - '0'; i++, col++)
                    at(idx(Square{col, row})) = Piece{Color::White, PieceType::None};
                break;

            case 'k':
            case 'K':
                if (c == 'k')
                    numBlackKings++;
                else
                    numWhiteKings++;
                [[fallthrough]];
            case 'p':
            case 'P':
            case 'n':
            case 'N':
            case 'b':
            case 'B':
            case 'r':
            case 'R':
            case 'q':
            case 'Q':
                at(idx(Square{col++, row})) = pieceMap[c];
                break;

            case '/':
                if (col != 8)
                    throw std::runtime_error("Invalid FEN string");
                row++;
                col = 0;
                break;

            case ' ':
                goto end;

            default:
                throw std::runtime_error("Invalid FEN string");
                break;
            }
        }
    end:
        if (row != 7 || col != 8 || numWhiteKings != 1 || numBlackKings != 1)
            throw std::runtime_error("Invalid FEN string");
    }

    Piece Position::get(Square square)
    {
        return at(idx(square));
    }

    Piece Position::put(Square square, Piece piece)
    {
        auto oldPiece = at(idx(square));
        at(idx(square)) = piece;
        return oldPiece;
    }

    inline uint8_t Position::idx(Square square)
    {
        return square.file + (7 - square.rank) * 8;
    }

    inline Square Position::idxToSquare(uint8_t idx)
    {
        return Square{static_cast<uint8_t>(idx % 8u), static_cast<uint8_t>(7u - idx / 8u)};
    }
} // namespace ChessGame