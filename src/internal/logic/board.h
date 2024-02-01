#pragma once

#include <array>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "internal/logic/move.h"
#include "internal/logic/offset.h"
#include "internal/logic/piece.h"
#include "internal/logic/square.h"

namespace ChessGame
{
    constexpr std::string_view initFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};

    class Occupant
    {
    public:
        constexpr Occupant() : m_occ(std::nullopt) {}
        constexpr Occupant(Piece piece) : m_occ(piece) {}
        constexpr Occupant(std::nullopt_t no) : m_occ(no) {}
        constexpr operator bool() const
        {
            return static_cast<bool>(m_occ);
        }
        operator Piece() = delete;
        explicit operator Piece() const
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
        constexpr std::array<Square, 64> eachSquare() const;
        constexpr const std::array<Occupant, 64> &eachOccupant() const;
        constexpr bool onBoard(Square square) const;
        constexpr size_t squareToIdx(Square square) const;
        constexpr Square idxToSquare(size_t idx) const;

    private:
        std::array<Occupant, 64> m_arr;
    };
} // namespace ChessGame
