#include "core/board.h"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace JChess
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

    bool Board::canMoveTo(Color movingColor, Square to) const
    {
        auto occ = get(to);
        return (!occ || occ.value().color != movingColor);
    }

    bool Board::pawnCanCapture(Color movingColor, Square to) const
    {
        auto occ = get(to);
        return (occ && occ.value().color != movingColor);
    }

    bool betweenSquares(Square test, Square start, Square end)
    {
        Offset dir1 = end - start;
        if (!dir1.isDiagonal() && !dir1.isLateral())
            return false;

        Offset dir2 = test - start;
        if (!dir2.isDiagonal() && !dir2.isLateral())
            return false;

        // return true iff (start < test < end) or (start > test > end) or (start == test == end)
        // for both rank and file, and both end-start and test-start are diagonal or lateral paths
        return ((start.file <=> test.file) == (test.file <=> end.file)) &&
               ((start.rank <=> test.rank) == (test.rank <=> end.rank));
    }

    std::vector<Square> Board::getPath(Square fromSquare, Offset direction, bool includePiece) const
    {
        std::vector<Square> path{};
        path.reserve(8);
        for (Square sq = fromSquare + direction; valid(sq); sq += direction)
        {
            if (get(sq))
            {
                if (includePiece)
                    path.push_back(sq);
                break;
            }
            path.push_back(sq);
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
                    fenstr += FEN::pieceToChar(m_arr[i].value());
                }
                else
                    ++num_empty;
            }
            if (num_empty)
                fenstr += static_cast<char>('0' + num_empty);
        }

        return fenstr;
    }

    std::array<Square, 64> Board::eachSquare()
    {
        std::array<Square, 64> squares;

        size_t i = 0;
        for (int rank = 7; rank >= 0; --rank)
        {
            for (int file = 0; file < 8; ++file, ++i)
            {
                squares[i].file = file;
                squares[i].rank = rank;
            }
        }

        return squares;
    }

    const std::array<Occupant, 64> &Board::eachOccupant() const
    {
        return m_arr;
    }

    size_t Board::squareToIdx(Square square)
    {
        return static_cast<size_t>((7 - square.rank) * 8 + square.file);
    }

    Square Board::idxToSquare(size_t idx)
    {
        int q = idx / 8, r = idx % 8;
        return Square{r, 7 - q};
    }

    bool Board::valid(Square square)
    {
        return 0 <= square.file &&
               square.file <= 7 &&
               0 <= square.rank &&
               square.rank <= 7;
    }

    int Board::homeRank(Color color)
    {
        return static_cast<int>(color) * 7;
    }

    int Board::rookFromFile(Castling::Side side)
    {
        // QS/0 -> 0
        // KS/1 -> 7
        return 7 * static_cast<int>(side);
    }

    int Board::rookToFile(Castling::Side side)
    {
        // QS/0 -> 3
        // KS/1 -> 5
        return 2 * static_cast<int>(side) + 3;
    }

    int Board::kingToFile(Castling::Side side)
    {
        // QS/0 -> 2
        // KS/1 -> 6
        return 4 * static_cast<int>(side) + 2;
    }

    Square Board::rookFromSquare(const Color color, Castling::Side side)
    {
        return Square{rookFromFile(side), homeRank(color)};
    }

    Square Board::rookToSquare(const Color color, Castling::Side side)
    {
        return Square{rookToFile(side), homeRank(color)};
    }

    Square Board::kingToSquare(const Color color, Castling::Side side)
    {
        return Square{kingToFile(side), homeRank(color)};
    }
} // namespace JChess