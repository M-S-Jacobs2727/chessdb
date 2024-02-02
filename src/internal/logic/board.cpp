#include "internal/logic/board.h"

#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <unordered_map>

namespace ChessGame
{
    Board::Board(std::string_view fenString)
    {
        uint8_t row = 0, col = 0, numWhiteKings = 0, numBlackKings = 0, i = 0;
        for (const auto &c : fenString)
        {
            bool complete{false};

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
                col += c - '0';
                i += c - '0';
                break;

            case 'p':
                m_arr[i++] = Piece{Color::Black, PieceType::Pawn};
                ++col;
                break;
            case 'P':
                m_arr[i++] = Piece{Color::White, PieceType::Pawn};
                ++col;
                break;
            case 'n':
                m_arr[i++] = Piece{Color::Black, PieceType::Knight};
                ++col;
                break;
            case 'N':
                m_arr[i++] = Piece{Color::White, PieceType::Knight};
                ++col;
                break;
            case 'b':
                m_arr[i++] = Piece{Color::Black, PieceType::Bishop};
                ++col;
                break;
            case 'B':
                m_arr[i++] = Piece{Color::White, PieceType::Bishop};
                ++col;
                break;
            case 'r':
                m_arr[i++] = Piece{Color::Black, PieceType::Rook};
                ++col;
                break;
            case 'R':
                m_arr[i++] = Piece{Color::White, PieceType::Rook};
                ++col;
                break;
            case 'q':
                m_arr[i++] = Piece{Color::Black, PieceType::Queen};
                ++col;
                break;
            case 'Q':
                m_arr[i++] = Piece{Color::White, PieceType::Queen};
                ++col;
                break;
            case 'k':
                numBlackKings++;
                m_arr[i++] = Piece{Color::Black, PieceType::King};
                ++col;
                break;
            case 'K':
                numWhiteKings++;
                m_arr[i++] = Piece{Color::White, PieceType::King};
                ++col;
                break;

            case '/':
                if (col != 8)
                    throw std::runtime_error("Invalid FEN string: wrong number of columns");
                row++;
                col = 0;
                break;

            case ' ':
                complete = true;
                break;

            default:
                throw std::runtime_error("Invalid FEN string: invalid character");
            }
            if (complete)
                break;
        }

        if (i != 64 || row != 7 || col != 8 || numWhiteKings != 1 || numBlackKings != 1)
            throw std::runtime_error("Invalid FEN string: end");
    }

    Occupant Board::get(Square square) const
    {
        return m_arr[squareToIdx(square)];
    }

    Occupant Board::put(Square square, Piece piece)
    {
        auto oldPiece = m_arr[squareToIdx(square)];
        m_arr[squareToIdx(square)] = piece;
        return oldPiece;
    }

    Occupant Board::remove(Square square)
    {
        auto p = get(square);
        m_arr[squareToIdx(square)] = std::nullopt;
        return p;
    }

    Square Board::kingSquare(Color color) const
    {
        auto it = std::find(m_arr.begin(), m_arr.end(), Piece{color, PieceType::King});
        if (it == m_arr.end())
            throw std::runtime_error("Could not locate king");

        size_t idx = static_cast<size_t>(it - m_arr.begin());
        return idxToSquare(idx);
    }

    std::vector<Square> Board::getPath(Square fromSquare, Offset direction, bool includePiece) const
    {
        std::vector<Square> path{};
        path.reserve(8);
        Square newSq = fromSquare;
        for (int i = 0; i < 8; ++i)
        {
            newSq += direction;
            if (onBoard(newSq))
                return path;

            auto occupant = get(newSq);
            if (!occupant || includePiece)
                path.push_back(newSq);
            if (occupant)
                return path;
        }
        return path;
    }

    constexpr std::array<Square, 64> Board::eachSquare() const
    {
        std::array<Square, 64> squares;

        size_t i = 0;
        for (int rank = 7; rank >= 0; ++rank)
        {
            for (int file = 0; file < 8; ++file, ++i)
            {
                squares[i].file = file;
                squares[i].rank = rank;
            }
        }

        return squares;
    }

    constexpr const std::array<Occupant, 64> &Board::eachOccupant() const
    {
        return m_arr;
    }

    constexpr size_t Board::squareToIdx(Square square) const
    {
        return static_cast<size_t>((7 - square.rank) * 8 + square.file);
    }

    constexpr Square Board::idxToSquare(size_t idx) const
    {
        size_t q = idx / 8, r = idx % 8;
        return Square{r, 7 - q};
    }

    constexpr bool Board::onBoard(Square square) const
    {
        return 0 <= square.file &&
               square.file <= 7 &&
               0 <= square.rank &&
               square.rank <= 7;
    }
} // namespace ChessGame