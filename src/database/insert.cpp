#include "database/insert.h"
#include "database/blobs.h"
#include "fileFormat/pgnFile.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <tuple>

namespace JChess
{
    uint64_t _insertGame(pqxx::work &txn, const Game &game)
    {
        auto movesBlob = movesToBlob(game.moves);
        constexpr std::array<std::string_view, 4> results{"white", "black", "draw", "none"};
        constexpr std::array<std::string_view, 8> conditions{
            "none", "checkmate", "timeout", "resignation",
            "infraction", "stalemate", "agreement", "material"};

        auto row = txn.exec_params1(
            "INSERT INTO games ("
            "gameID, datetimeUTC, whiteUsername, blackUsername, whiteELO, blackELO, "
            "timeLimit, timeIncrement, winner, condition, halfMoves, ecoCode, movesBlob)"
            "VALUES ("
            "DEFAULT, timestamp (0) '$1$2$3 $4$5$6', '$7', '$8', $9, $10,"
            "interval '$11 seconds', interval '$12 seconds', '$13', '$14', $15, '$16', $17)"
            "RETURNING gameID;",
            game.datetime.year, game.datetime.month, game.datetime.day, game.datetime.hour, game.datetime.minute, game.datetime.second,
            game.whiteUsername, game.blackUsername, game.whiteELO, game.blackELO, game.timeControl.initialTime, game.timeControl.increment,
            results[static_cast<int>(game.result.type)], conditions[static_cast<int>(game.result.reason)],
            game.moves.size(), game.ECOCode, movesBlob);

        auto [gameID] = row.as<uint64_t>();
        return gameID;
    }

    std::set<blob> _getPositionSet(const Game &game)
    {
        std::set<blob> posBlobs{};
        for (const auto &state : game.states)
            posBlobs.insert(positionToBlob(state.board));
        return posBlobs;
    }

    pqxx::result _insertPositions(pqxx::work &txn, const std::set<blob> &posSet)
    {
        std::string cmd{"MERGE INTO positions AS pos USING (VALUES"};
        pqxx::params params;
        pqxx::placeholders name;
        for (const auto &pos : posSet)
        {
            params.append(pos);
            cmd += " (" + name.get() + "),";
            name.next();
        }
        cmd.back() = ')'; // Replace last comma with closing parens
        cmd += " AS tmp ON pos.board = tmp.column1 WHEN NOT MATCHED THEN INSERT (board) VALUES (tmp.column1);";

        return txn.exec_params(cmd, params);
    }

    std::vector<uint64_t> _selectPositionIDs(pqxx::work &txn,
                                             const std::set<blob> &posSet,
                                             const std::vector<blob> &posVec)
    {
        std::string cmd{"SELECT positionID, board FROM positions WHERE positions.board IN ("};
        pqxx::params params;
        pqxx::placeholders name;
        for (const auto &pos : posSet)
        {
            params.append(pos);
            cmd += name.get() + ", ";
            name.next();
        }
        cmd[cmd.size() - 2] = ')'; // Replace last comma with closing parens
        cmd.back() = ';';

        auto result = txn.exec_params_n(posSet.size(), cmd, params);

        std::vector<uint64_t> posIDs{posVec.size()};
        for (size_t i = 0; i < posVec.size(); ++i)
        {
            for (const auto &row : result)
            {
                auto [id, b] = row.as<uint64_t, blob>();
                if (posVec[i] == b)
                {
                    posIDs[i] = id;
                    break;
                }
            }
        }
        return posIDs;
    }

    void _moveToParams(pqxx::params &params,
                       const Move &move,
                       const State &state,
                       const std::optional<ClockTime> &clock,
                       const std::optional<Annotation> &annotation,
                       const std::optional<Evaluation> &evaluation)
    {
        params.append(toStr(move.piece.type).value());
        params.append(move.from.str());
        params.append(move.to.str());
        params.append(move.check);
        params.append(toStr(move.capturedPiece.type));
        params.append(toStr(move.promotedPiece.type));
        params.append(toStr(annotation));
        params.append(clock ? std::make_optional(clock.value().seconds) : std::nullopt);
        std::optional<float> evalCP = std::nullopt;
        std::optional<int> evalM = std::nullopt;
        if (evaluation)
        {
            auto e = evaluation.value().value;
            if (e.index() == 0) // int, mate
                evalM = std::get<int>(e);
            else
                evalCP = std::get<float>(e);
        }
        params.append(evalCP);
        params.append(evalM);
        params.append(state.castleRights.str());
        params.append(state.enPassant ? std::make_optional(state.enPassant.value().str()) : std::nullopt);
        params.append(state.halfTurnCounter);
    }

    void _insertMoves(pqxx::work &txn,
                      const Game &game,
                      uint64_t gameID,
                      const std::vector<uint64_t> &posIDs)
    {
        using namespace std::string_view_literals;

        auto stream = pqxx::stream_to(
            txn,
            "moves"sv,
            std::array<std::string_view, 19>(
                {"gameID"sv,
                 "prevMoveID"sv,
                 "nextMoveID"sv,
                 "positionID"sv,
                 "moveNumber"sv,
                 "activeColor"sv,
                 "piece"sv,
                 "fromSquare"sv,
                 "toSquare"sv,
                 "inCheck"sv,
                 "capturedPiece"sv,
                 "promotedPiece"sv,
                 "annotation"sv,
                 "timeLeft"sv,
                 "evaluationCP"sv,
                 "evaluationM"sv,
                 "castling"sv,
                 "enPassant"sv,
                 "halfMoveCount"sv}));

        for (size_t i = 0; i < game.moves.size(); ++i)
        {
            pqxx::params params;
            params.append(gameID);
            params.append(std::nullopt);
            params.append(std::nullopt);
            params.append(posIDs[i]);
            params.append(i + 1);
            params.append((i % 2) ? "black"sv : "white"sv);
            _moveToParams(params,
                          game.moves[i],
                          game.states[i],
                          game.clocks ? std::make_optional(game.clocks.value()[i]) : std::nullopt,
                          game.annotations ? std::make_optional(game.annotations.value()[i]) : std::nullopt,
                          game.evaluations ? std::make_optional(game.evaluations.value()[i]) : std::nullopt);
            stream.write_values(params);
        }
        stream.complete();
    }

    void _setMoveRefs(pqxx::work &txn, const uint64_t gameID, const uint64_t numMoves)
    {
        auto result = txn.exec_params_n(
            numMoves,
            "SELECT moveID FROM moves WHERE gameID == $1 ORDERBY moveNumber;",
            gameID);

        std::vector<uint64_t> moveIDs;
        moveIDs.resize(numMoves);
        std::ranges::transform(
            result,
            moveIDs.begin(),
            [](const pqxx::result::reference &row)
            { return std::get<0>(row.as<uint64_t>()); });

        // update id[i] with prevID=id[i-1] and nextID=id[i+1]
        txn.exec_params1("UPDATE moves SET nextMoveID = $1 WHERE moveID = $2;", moveIDs[1], moveIDs.front());
        txn.exec_params1("UPDATE moves SET prevMoveID = $1 WHERE moveID = $2;", moveIDs[numMoves - 2], moveIDs.back());
        for (size_t i = 1; i < numMoves - 1; ++i)
            txn.exec_params1("UPDATE moves SET prevMoveID = $1, nextMoveID = $2 WHERE moveID = $3;", moveIDs[i - 1], moveIDs[i + 1], moveIDs[i]);
    }

    void insertGame(pqxx::connection &conn, const Game &game)
    {
        using namespace std::string_literals;
        using namespace std::string_view_literals;

        constexpr std::array<std::string_view, 4> results{"white", "black", "draw", "none"};
        constexpr std::array<std::string_view, 8> conditions{
            "none", "checkmate", "timeout", "resignation",
            "infraction", "stalemate", "agreement", "material"};

        try
        {
            pqxx::work txn(conn);
            uint64_t gameID = _insertGame(txn, game);

            std::vector<blob> posVec;
            posVec.reserve(game.states.size());
            for (const auto &state : game.states)
                posVec.push_back(positionToBlob(state.board));
            std::set<blob> posSet(posVec.begin(), posVec.end());

            _insertPositions(txn, posSet);
            auto posIDs = _selectPositionIDs(txn, posSet, posVec);

            _insertMoves(txn, game, gameID, posIDs);
            _setMoveRefs(txn, gameID, game.moves.size());

            txn.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
} // namespace JChess
