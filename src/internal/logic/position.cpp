#include "internal/logic/position.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ChessGame
{
    const std::unordered_map<char, Piece> FENPiece = {
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

    Position::Position(std::string_view fenString)
    {
        m_arr.fill(std::nullopt);
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
                m_arr[i++] = FENPiece.at(c);
                ++col;
                break;

            case '/':
                if (col != 8)
                    throw std::runtime_error("Invalid FEN string");
                row++;
                col = 0;
                break;

            case ' ':
                complete = true;

            default:
                throw std::runtime_error("Invalid FEN string");
            }
            if (complete)
                break;
        }

        if (i != 63 || row != 7 || col != 8 || numWhiteKings != 1 || numBlackKings != 1)
            throw std::runtime_error("Invalid FEN string");
    }

    inline std::optional<Piece> Position::get(Square square) const
    {
        return m_arr[square.idx()];
    }

    std::optional<Piece> Position::put(Square square, Piece piece)
    {
        auto oldPiece = m_arr[square.idx()];
        m_arr[square.idx()] = piece;
        return oldPiece;
    }

    std::optional<Piece> Position::remove(Square square)
    {
        auto p = get(square);
        m_arr[square.idx()] = std::nullopt;
        return p;
    }

    constexpr Square Position::kingSquare(Color color) const
    {
        for (const auto &[sq, p] : eachSquare())
            if (p && p.value().type == PieceType::King && p.value().color == color)
                return sq;
        throw std::runtime_error("Could not locate king");
    }

    /*
     * Returns a (possibly empty) vector of `Square`s along a given direction,
     * up to and optionally including the first piece found.
     */
    std::vector<Square> Position::getPath(Square fromSquare, Offset direction, bool includePiece) const
    {
        std::vector<Square> path{};
        path.reserve(8);
        std::optional<Square> newSq = fromSquare;
        for (int i = 0; i < 8; ++i)
        {
            newSq = direction(newSq.value());
            if (!newSq)
                return path;

            auto occupant = get(newSq.value());
            if (!occupant || includePiece)
                path.push_back(newSq.value());
            if (occupant)
                return path;
        }
        return path;
    }

    std::array<std::pair<Square, std::optional<Piece>>, 64> Position::eachSquare() const
    {
        std::array<std::pair<Square, std::optional<Piece>>, 64> squaresAndPieces{};
        for (uint8_t rank = 0; rank < 8; ++rank)
        {
            for (uint8_t file = 0; file < 8; ++file)
            {
                Square sq{file, rank};
                auto p = get(sq);
                squaresAndPieces[sq.idx()] = {sq, p};
            }
        }

        return squaresAndPieces;
    }
} // namespace ChessGame