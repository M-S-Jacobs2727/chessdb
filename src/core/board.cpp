#include "core/board.h"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace ChessGame
{
    Board::Board(std::string_view fenString)
    {
        uint8_t row = 0, col = 0, numWhiteKings = 0, numBlackKings = 0, i = 0;
        for (const auto &c : fenString)
        {
            if (c == ' ')
                break;

            auto p = FEN::charToPiece(c);
            Piece piece;
            int num_empty;

            switch (p.index())
            {
            case 0:
                piece = std::get<Piece>(p);
                m_arr[i++] = piece;
                ++col;

                if (piece.type == PieceType::King)
                {
                    switch (piece.color)
                    {
                    case Color::White:
                        ++numWhiteKings;
                        break;
                    case Color::Black:
                        ++numBlackKings;
                        break;

                    default:
                        std::unreachable();
                        break;
                    }
                }
                break;

            case 1:
                num_empty = std::get<int>(p);
                if (num_empty)
                {
                    col += num_empty;
                    i += num_empty;
                }
                else
                {
                    if (col != 8)
                        throw std::runtime_error("Invalid FEN string: wrong number of columns");
                    if (row == 7)
                        throw std::runtime_error("Invalid FEN string: too many rows");

                    ++row;
                    col = 0;
                }
                break;

            default:
                std::unreachable();
                break;
            }
        }

        if (i != 64 || row != 7 || col != 8 || numWhiteKings != 1 || numBlackKings != 1)
        {
            std::ostringstream err{"Invalid FEN string: "};
            if (row != 7)
                err << "only " << row << " rows.";
            else if (col != 8)
                err << "last row has " << col << " columns.";
            else if (numWhiteKings != 1 || numBlackKings != 1)
                err << "found " << numWhiteKings << " white kings and " << numBlackKings << " black kings.";
            else
                err << "i = " << i << '.';
            throw std::runtime_error(err.str());
        }
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
            if (valid(newSq))
                return path;

            auto occupant = get(newSq);
            if (!occupant || includePiece)
                path.push_back(newSq);
            if (occupant)
                return path;
        }
        return path;
    }

    std::string Board::toFen() const
    {
        std::string fenstr;
        fenstr.reserve(75);

        for (size_t row = 0, i = 0; row < 8; ++row)
        {
            if (row != 0)
                fenstr += '/';

            size_t num_empty = 0;
            for (size_t col = 0; col < 8; ++col, ++i)
            {
                if (m_arr[i])
                {
                    if (num_empty)
                        fenstr += static_cast<char>('0' + num_empty);
                    fenstr += FEN::pieceToChar(m_arr[i].piece());
                }
                else
                    ++num_empty;
            }
            if (num_empty)
                fenstr += static_cast<char>('0' + num_empty);
        }

        return fenstr;
    }

    constexpr std::array<Square, 64> Board::eachSquare()
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

    constexpr size_t Board::squareToIdx(Square square)
    {
        return static_cast<size_t>((7 - square.rank) * 8 + square.file);
    }

    constexpr Square Board::idxToSquare(size_t idx)
    {
        size_t q = idx / 8, r = idx % 8;
        return Square{r, 7 - q};
    }

    constexpr bool Board::valid(Square square)
    {
        return 0 <= square.file &&
               square.file <= 7 &&
               0 <= square.rank &&
               square.rank <= 7;
    }

    constexpr int Board::homeRank(Color color)
    {
        return static_cast<int>(color) * 7;
    }

    constexpr int Board::rookFromFile(Castling::Side side)
    {
        // QS/0 -> 0
        // KS/1 -> 7
        return 7 * static_cast<int>(side);
    }

    constexpr int Board::rookToFile(Castling::Side side)
    {
        // QS/0 -> 3
        // KS/1 -> 5
        return 2 * static_cast<int>(side) + 3;
    }

    constexpr int Board::kingToFile(Castling::Side side)
    {
        // QS/0 -> 2
        // KS/1 -> 6
        return 4 * static_cast<int>(side) + 2;
    }

    constexpr Square Board::rookFromSquare(const Color color, Castling::Side side)
    {
        return Square{rookFromFile(side), homeRank(color)};
    }

    constexpr Square Board::rookToSquare(const Color color, Castling::Side side)
    {
        return Square{rookToFile(side), homeRank(color)};
    }

    constexpr Square Board::kingToSquare(const Color color, Castling::Side side)
    {
        return Square{kingToFile(side), homeRank(color)};
    }
} // namespace ChessGame