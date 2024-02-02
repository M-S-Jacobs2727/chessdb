#pragma once

#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "core/castling.h"
#include "core/color.h"
#include "core/offset.h"
#include "core/piece.h"
#include "core/square.h"

namespace ChessGame
{
    constexpr std::string_view initFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    class Occupant
    {
    public:
        constexpr Occupant() : m_occ(std::nullopt) {}
        constexpr operator bool() const
        {
            return static_cast<bool>(m_occ);
        }
        operator Piece() = delete;
        void operator=(Piece piece)
        {
            m_occ = piece;
        }
        void operator=(std::nullopt_t no)
        {
            m_occ = std::nullopt;
        }
        const Piece &piece() const
        {
            return m_occ.value();
        }

    private:
        std::optional<Piece> m_occ;
    };

    class Board
    {
    public:
        explicit Board(std::string_view fenString);
        Occupant get(Square square) const;
        Occupant put(Square square, Piece piece);
        Occupant remove(Square square);
        Square kingSquare(Color color) const;

        /*
         * Returns a (possibly empty) vector of `Square`s along a given direction,
         * up to and optionally including the first piece found.
         */
        std::vector<Square> getPath(Square fromSquare, Offset direction, bool includePiece) const;
        constexpr const std::array<Occupant, 64> &eachOccupant() const;

        static constexpr std::array<Square, 64> eachSquare();

        static constexpr bool valid(Square square);
        static constexpr size_t squareToIdx(Square square);
        static constexpr Square idxToSquare(size_t idx);
        static constexpr int homeRank(Color color);

        static constexpr int rookFromFile(Castling::Side side);
        static constexpr int rookToFile(Castling::Side side);
        static constexpr int kingToFile(Castling::Side side);
        static constexpr Square rookFromSquare(Color color, Castling::Side side);
        static constexpr Square rookToSquare(Color color, Castling::Side side);
        static constexpr Square kingToSquare(Color color, Castling::Side side);

    private:
        std::array<Occupant, 64> m_arr;
    };
} // namespace ChessGame
