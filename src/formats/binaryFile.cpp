#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <functional>
#include <variant>

#include "fileFormat/binaryFile.h"

namespace JChess
{
    constexpr std::string_view FORMATCODE = "PGN1";

    template <class T, class U>
    constexpr void writeToBits(T &bits, U value, uint8_t shift)
    {
        bits <<= shift;
        bits |= static_cast<T>(value);
    }

    template <class T, class U>
    constexpr T readFromBits(T &bits, U mask, uint8_t shift)
    {
        T value = bits & static_cast<T>(mask);
        bits >>= shift;
        return value;
    }

    void writeBinary(std::ostream &output, const Game &game)
    {
        writeBinaryHeader(output, game);
        for (const auto &m : game.moves)
            writeBinaryMove(output, m);

        if (game.evaluations)
            for (const auto &e : game.evaluations.value())
                writeBinaryEvaluation(output, e.value);

        if (game.clocks)
            for (const auto &c : game.clocks.value())
                writeBinaryClock(output, c.seconds);
    }

    void writeBinaryHeader(std::ostream &output, const Game &game)
    {
        output.write(FORMATCODE.data(), 4);
        uint16_t whiteUsernameLength = game.whiteUsername.size();
        uint16_t blackUsernameLength = game.blackUsername.size();
        output.write(reinterpret_cast<const char *>(&whiteUsernameLength), sizeof(whiteUsernameLength));
        output.write(reinterpret_cast<const char *>(&blackUsernameLength), sizeof(blackUsernameLength));
        output.write(game.whiteUsername.c_str(), whiteUsernameLength);
        output.write(game.blackUsername.c_str(), blackUsernameLength);
        char formattedUTCDatetime[15];
        snprintf(formattedUTCDatetime, 15, "%04d%02d%02d%02d%02d%02d",
                 game.datetime.year, game.datetime.month, game.datetime.day,
                 game.datetime.hour, game.datetime.minute, game.datetime.second);
        output.write(formattedUTCDatetime, 14);

        output.write(reinterpret_cast<const char *>(&game.whiteELO), sizeof(game.whiteELO));
        output.write(reinterpret_cast<const char *>(&game.blackELO), sizeof(game.blackELO));
        output.write(game.ECOCode.c_str(), 3);
        output.write(reinterpret_cast<const char *>(&game.timeControl.initialTime), sizeof(game.timeControl.initialTime));
        output.write(reinterpret_cast<const char *>(&game.timeControl.increment), sizeof(game.timeControl.increment));

        uint8_t status = 0;
        writeToBits(status, game.evaluations.has_value(), 1);
        writeToBits(status, game.clocks.has_value(), 1);
        writeToBits(status, game.result.type, 2);
        writeToBits(status, game.result.reason, 3);
        output.write(reinterpret_cast<const char *>(&status), sizeof(status));

        uint16_t numberOfHalfMoves = game.moves.size();
        output.write(reinterpret_cast<const char *>(&numberOfHalfMoves), sizeof(numberOfHalfMoves));
    }

    void writeBinaryMove(std::ostream &output, const Move &move)
    {
        uint16_t move_data = 0;
        writeToBits(move_data, move.piece.color, 1);
        writeToBits(move_data, move.piece.type, 3);
        writeToBits(move_data, move.from.file, 3);
        writeToBits(move_data, move.from.rank, 3);
        writeToBits(move_data, move.to.file, 3);
        writeToBits(move_data, move.to.rank, 3);
        output.write(reinterpret_cast<const char *>(&move_data), sizeof(move_data));

        uint8_t extra_data = 0;
        writeToBits(extra_data, move.check, 1);
        writeToBits(extra_data, move.capturedPiece, 3);
        // writeToBits(extra_data, move.annotation, 3);
        output.write(reinterpret_cast<const char *>(&extra_data), sizeof(extra_data));
    }

    void writeBinaryEvaluation(std::ostream &output, const eval_t &eval)
    {
        if (const auto eval_ptr = std::get_if<float>(&eval))
            output.write(reinterpret_cast<const char *>(eval_ptr), sizeof(*eval_ptr));
        else if (const auto mate_evaluation_ptr = std::get_if<int>(&eval))
        {
            uint32_t tmp = (*mate_evaluation_ptr) | 0xFFFFFF00;
            output.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        }
        else
            throw std::runtime_error("Invalid evaluation type");
    }

    void writeBinaryClock(std::ostream &output, const uint16_t secondsRemaining)
    {
        output.write(reinterpret_cast<const char *>(&secondsRemaining), sizeof(secondsRemaining));
    }

    void writeBinaryState(std::ostream &output, const State &board)
    {
        uint32_t row = 0;
        size_t idx = 0;
        for (auto i = 0; i < 8; ++i)
        {
            for (auto j = 0; j < 8; ++j)
            {
                auto piece = board.board[idx++];
                row <<= 1;
                row |= static_cast<uint8_t>(piece.color);
                row <<= 3;
                row |= static_cast<uint8_t>(piece.type);
            }
            output.write(reinterpret_cast<const char *>(&row), sizeof(row));
        }

        std::bitset<8> extra{0u};
        if (board.enPassant)
            extra |= static_cast<uint8_t>(board.enPassant.value().file);
        extra[7] = board.turn == Color::Black;
        extra[6] = board.castleRights.get(Color::White, CastleRights::Side::KING);
        extra[5] = board.castleRights.get(Color::White, CastleRights::Side::QUEEN);
        extra[4] = board.castleRights.get(Color::Black, CastleRights::Side::KING);
        extra[3] = board.castleRights.get(Color::Black, CastleRights::Side::QUEEN);
        uint8_t extra_byte = static_cast<uint8_t>(extra.to_ulong());
        output.write(reinterpret_cast<const char *>(&extra_byte), sizeof(extra_byte));

        auto tmp = board.halfTurnCounter;
        if (board.enPassant)
            tmp = std::numeric_limits<uint8_t>::max();
        output.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));

        output.write(reinterpret_cast<const char *>(&board.fullTurnCounter), sizeof(board.fullTurnCounter));
    }

    Game readBinary(std::istream &input)
    {
        Game game;
        readBinaryHeader(input, game);
        for (auto &move : game.moves)
            readBinaryMove(input, move);

        if (game.evaluations)
            for (auto &eval : game.evaluations.value())
                readBinaryEvaluation(input, eval);

        if (game.clocks)
            for (auto &clock : game.clocks.value())
                readBinaryClock(input, clock);

        return game;
    }

    void readBinaryHeader(std::istream &input, Game &game)
    {
        std::string formatID{"0000"};
        input.read(formatID.data(), 4);
        if (formatID != FORMATCODE)
            throw std::runtime_error("Invalid binary format code");

        uint16_t whiteUsernameLength = 0, blackUsernameLength = 0;
        input.read(reinterpret_cast<char *>(&whiteUsernameLength), sizeof(whiteUsernameLength));
        input.read(reinterpret_cast<char *>(&blackUsernameLength), sizeof(blackUsernameLength));
        game.whiteUsername.resize(whiteUsernameLength);
        game.blackUsername.resize(blackUsernameLength);
        input.read(game.whiteUsername.data(), whiteUsernameLength);
        input.read(game.blackUsername.data(), blackUsernameLength);

        std::string datetimeString;
        datetimeString.resize(14);
        input.read(datetimeString.data(), 14);
        game.datetime = Datetime(datetimeString.substr(0, 8), datetimeString.substr(8));

        input.read(reinterpret_cast<char *>(&game.whiteELO), sizeof(game.whiteELO));
        input.read(reinterpret_cast<char *>(&game.blackELO), sizeof(game.blackELO));

        input.read(game.ECOCode.data(), 3);

        input.read(reinterpret_cast<char *>(&game.timeControl.initialTime), sizeof(game.timeControl.initialTime));
        input.read(reinterpret_cast<char *>(&game.timeControl.increment), sizeof(game.timeControl.increment));

        uint8_t status = 0;
        input.read(reinterpret_cast<char *>(&status), sizeof(status));
        game.result.reason = static_cast<GameResult::Reason>(readFromBits(status, 0x07u, 3));
        game.result.type = static_cast<GameResult::Type>(readFromBits(status, 0x03u, 2));
        bool includeClocks = readFromBits(status, 0x01u, 1);
        bool includeEvals = readFromBits(status, 0x01u, 1);
        if (includeEvals)
            game.evaluations.emplace();
        if (includeClocks)
            game.clocks.emplace();

        uint16_t numberOfHalfMoves = 0;
        input.read(reinterpret_cast<char *>(&numberOfHalfMoves), sizeof(numberOfHalfMoves));
        game.moves.resize(numberOfHalfMoves);
    }

    void readBinaryMove(std::istream &input, Move &move)
    {
        uint16_t move_data = 0;
        uint8_t extra_data = 0;

        input.read(reinterpret_cast<char *>(&move_data), sizeof(move_data));
        input.read(reinterpret_cast<char *>(&extra_data), sizeof(extra_data));

        move.to.file = readFromBits(move_data, 0x07u, 3);
        move.to.rank = readFromBits(move_data, 0x07u, 3);

        move.from.file = readFromBits(move_data, 0x07u, 3);
        move.from.rank = readFromBits(move_data, 0x07u, 3);

        move.piece.type = static_cast<PieceType>(readFromBits(move_data, 0x07u, 3));
        move.piece.color = static_cast<Color>(move_data);

        // move.annotation = static_cast<Annotation>(readFromBits(extra_data, 0x07u, 3));
        move.capturedPiece = Piece{.color = oppositeColor(move.piece.color),
                                   .type = static_cast<PieceType>(readFromBits(extra_data, 0x07u, 3))};
        move.check = static_cast<bool>(readFromBits(extra_data, (uint8_t)0x01, 1));
    }

    void readBinaryEvaluation(std::istream &input, Evaluation &eval)
    {
        float tmp = 0;
        input.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (!std::isnan(tmp))
        {
            eval.value = tmp;
            return;
        }

        int32_t e = 0;
        std::memcpy(&e, &tmp, 4);
        eval.value = static_cast<int>(e);
    }

    void readBinaryClock(std::istream &input, ClockTime &clock)
    {
        input.read(reinterpret_cast<char *>(&clock.seconds), sizeof(clock.seconds));
    }

    void readBinaryState(std::istream &input, State &board)
    {
        uint32_t row = 0;
        size_t idx = 0;
        for (auto i = 0; i < 8; ++i)
        {
            input.read(reinterpret_cast<char *>(&row), sizeof(row));
            for (auto j = 0; j < 8; ++j)
            {
                board.board[idx].color = static_cast<Color>((row & 0x80000000) >> 31);
                board.board[idx++].type = static_cast<PieceType>((row & 0x70000000) >> 28);
                row <<= 4;
            }
        }

        uint8_t extra = 0;
        input.read(reinterpret_cast<char *>(&extra), sizeof(extra));
        std::bitset<8> extraBits{extra};
        board.turn = static_cast<Color>(extraBits.test(7));
        board.castleRights.set(Color::White, CastleRights::Side::KING, extraBits.test(6));
        board.castleRights.set(Color::White, CastleRights::Side::QUEEN, extraBits.test(5));
        board.castleRights.set(Color::Black, CastleRights::Side::KING, extraBits.test(4));
        board.castleRights.set(Color::Black, CastleRights::Side::QUEEN, extraBits.test(3));
        board.enPassant.emplace("a1");
        board.enPassant.value().file = extra & 0x07;

        input.read(reinterpret_cast<char *>(&board.halfTurnCounter), sizeof(board.halfTurnCounter));
        if (board.halfTurnCounter != std::numeric_limits<uint8_t>::max())
            board.halfTurnCounter = 0u;
        else
            board.enPassant.reset();

        input.read(reinterpret_cast<char *>(&board.fullTurnCounter), sizeof(board.fullTurnCounter));
    }
} // namespace ChessGame