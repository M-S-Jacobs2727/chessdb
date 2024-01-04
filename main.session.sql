CREATE TYPE color AS ENUM ('white', 'black');
CREATE TYPE gameWinner AS ENUM ('none', 'white', 'black', 'draw');
CREATE TYPE winCondition AS ENUM (
    'none',
    'checkmate',
    'timeout',
    'resignation',
    'infraction',
    'stalemate',
    'agreement',
    'material',
);
CREATE TYPE pieceType AS ENUM (
    'pawn',
    'knight',
    'bishop',
    'rook',
    'queen',
    'king'
);
CREATE TYPE annotationType AS ENUM (
    'mistake',
    'blunder',
    'good',
    'brilliant',
    'interesting',
    'dubious'
);
CREATE TYPE castleRights AS (
    whiteKingside BOOLEAN,
    whiteQueenside BOOLEAN,
    blackKingside BOOLEAN,
    blackQueenside BOOLEAN
);
CREATE TABLE IF NOT EXISTS games (
    gameID INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    datetimeUTC TIMESTAMP (0),
    whiteUsername TEXT,
    blackUsername TEXT,
    whiteELO INTEGER CHECK (
        whiteELO BETWEEN 100 AND 6000
    ),
    blackELO INTEGER CHECK (
        blackELO BETWEEN 100 AND 6000
    ),
    timeLimit INTERVAL CHECK (timeLimit > '0 seconds'),
    timeIncrement INTERVAL CHECK (timeIncrement >= '0 seconds'),
    winner gameWinner NOT NULL,
    condition winCondition,
    halfMoves INTEGER NOT NULL CHECK (halfMoves >= 0),
    ecoCode VARCHAR(3) CHECK (char_length(ecoCode) = 3),
    movesBlob BYTEA NOT NULL
);
CREATE TABLE IF NOT EXISTS positions (
    positionID INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    position BYTEA NOT NULL
);
CREATE TABLE IF NOT EXISTS moves (
    moveID INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    gameID INTEGER REFERENCES games ON UPDATE CASCADE ON DELETE CASCADE,
    prevMoveID INTEGER REFERENCES moves ON UPDATE CASCADE ON DELETE
    SET NULL,
        nextMoveID INTEGER REFERENCES moves ON UPDATE CASCADE ON DELETE
    SET NULL,
        positionID INTEGER REFERENCES positions ON UPDATE CASCADE ON DELETE
    SET NULL,
        moveNumber INTEGER NOT NULL DEFAULT 1 CHECK (moveNumber > 0),
        activeColor color NOT NULL,
        piece pieceType NOT NULL,
        fromSquare VARCHAR(2) NOT NULL CHECK (regexp_like(fromSquare, '[a-h][1-8]')),
        toSquare VARCHAR(2) NOT NULL CHECK (regexp_like(toSquare, '[a-h][1-8]')),
        inCheck BOOLEAN NOT NULL DEFAULT FALSE,
        capturedPiece pieceType,
        promotedPiece pieceType,
        annotation annotationType,
        timeLeft INTERVAL,
        evaluationCP NUMERIC(6, 2) DEFAULT 0.00,
        evaluationM INTEGER,
        castling castleRights,
        enPassant VARCHAR(1) CHECK(regexp_like(enPassant, '[a-h]')),
        halfMoveCount INTEGER NOT NULL DEFAULT 0 CHECK(halfMoveCount >= 0)
);
SELECT *
FROM games;
SELECT *
FROM moves;
SELECT *
FROM positions;
INSERT INTO games (
        gameID,
        datetimeUTC,
        whiteUsername,
        blackUsername,
        whiteELO,
        blackELO,
        timeLimit,
        timeIncrement,
        winner,
        condition,
        halfMoves,
        ecoCode,
        movesBlob
    )
VALUES (
        DEFAULT,
        TIMESTAMP (0) '20240101 00:01:02',
        'wu',
        'bu',
        1000,
        1100,
        INTERVAL '120 seconds',
        INTERVAL '0 seconds',
        'black',
        'checkmate',
        64,
        'A01',
        BYTEA '\\x0000000011111111'
    );
MERGE INTO positions AS pos USING (
    VALUES (
            BYTEA '\\xCABDEBAC999999990000000000000000000000001111111142356324'
        ),
        (
            BYTEA '\\xCABDEBAC999999990000000000000000000000001111111142356324'
        ),
        (
            BYTEA '\\xCABDEBAC999999990000000000000000000000001111111142356324'
        ),
        (
            BYTEA '\\xCABDEBAC999999990000000000000000000000001111111142356324'
        ),
        (
            BYTEA '\\xCABDEBAC999999990000000000000000000000001111111142356324'
        )
) AS tmp ON pos.position = tmp.column1
WHEN NOT MATCHED THEN
INSERT (position)
VALUES (tmp.column1);