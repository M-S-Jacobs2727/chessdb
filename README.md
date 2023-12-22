# chessdb

## Stockfish Engine

Download the source code at https://github.com/official-stockfish/Stockfish/

## PGN Format

### Header
1. Event (optional)
2. Site (optional)
3. Date (YYYY.mm.dd) (redundant with 8.)
4. Round (for multi-round matches)
5. White player username
6. Black player username
7. Result (0-1, 1-0, 1/2-1/2)
8. UTCDate
9. UTCTime
10. White ELO
11. Black ELO
12. White Rating Diff (optional)
13. Black Rating Diff (optional)
14. ECO (Opening code)
15. Opening (optional)
16. Time Control
17. Termination ("Normal", "Time Forfeit", "Abandoned", "Rules Infraction", "Unterminated")
    a. Unterminated games do not have times, should ignore
    b. Abandoned games have <3 moves, should ignore

### Move Notations
\d+\.(\.\.)? [\w\d\+#]+[?!]?[?!]? (\{ (\[%eval ((#-?\d+)|(-?\d+\.\d+))\])? (\[%clk \d+:\d\d:\d\d\])? \})?

### Result
(0-1)|(1-0)|(1/2-1/2)

## Binary Format
Ints are unsigned unless noted

### Header (36 bytes)
1. format ID (4 bytes, first one non-unicode)
2. white username length (16-bit int)
3. black username length (16-bit int)
4. white username
5. black username
6. Formatted UTCDatetime (YYYYmmddHHMMSS, 14 chars)
7. White ELO (16-bit int)
8. Black ELO (16-bit int)
9. ECO code (3 chars)
10. Time control (16-bit int, 16-bit int)
11. Status (8 bits)
    a. Unused bit (1 bit bool)
    b. Include Evaluation (1 bit bool)
    c. Include Clock (1 bit bool)
    d. Result (2 bits, (W, b, draw, null))
    e. Reason (3 bits, 4 for win, 4 for draw)
        i. Win: Normal (checkmate or regignation), Time forfeit, Rules infraction
        ii. Draw: Normal (stalemate, time v. insufficient material, 3-fold rep, 50 move rule)
        iii. Read from file: Normal, Time forfeit, Rules infraction, Unterminated, Abandoned 
15. Number of half-moves (16-bit int)

### Move Notations (3-9 bytes per move)
1. Move (2 bytes)
    a. Piece ID (4 bits)
        i. MSB for color (0, 1) -> (W, b)
        ii. 3 LSBs for piece (0-7) -> (null, p, n, b, r, q, k, null2)
    b. Start square (6 bits)
        i. Column (file) (3 bits, 0-7 -> a-h)
        ii. Row (rank) (3 bits, 0-7 -> 1-8)
    c. End square (6 bits)
        i. Column (file) (3 bits, 0-7 -> a-h)
        ii. Row (rank) (3 bits, 0-7 -> 1-8)
        iii. Exception: Promotion
            1. Either (a.i is black pawn and b.ii is rank 2) or (a.i is white pawn and b.ii is rank 7)
            2. 2 LSBs from c.ii describe promotion piece (0-4) -> (n, b, r, q)
2. Extra info (1 byte)
    a. Unused bit (1 bit)
    b. Check (1 bit bool)
    c. Capture (3 bits)
        i. (0-7) -> (none, p, n, b, r, q, ?, ?)
    d. Annotation (3 bits)
        i. (0-7) -> (null, ?, ??, ?!, !, !?, !!)
6. Evaluation (4 bytes) (optional, if H11a)
    a. 32-bit float (positive for white advantage, negative for black)
    b. Can be NaN (exponent is 0b11111111), signals mate-in-X
        i. 8 LSBs in 23-bit significand -> signed int X
7. Clock (2 bytes) (optional, if H11b)
    a. Seconds as 16-bit int

## PGN Interpreter

### Starting position

rnbkqbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBKQBNR

### Piece moves to square

1. Known: PieceType and ToSquare
2. Need to know: FromSquare and CaptureType

### Board state (described by FEN or similar)

1. Pieces on board (32B if contiguous)
2. Full turn counter (unnecessary?, storing sequence of states)
3. Half turn counter for 50 move rule (reset on pawn move, capture, ?) (1 byte, int8_t, sign for turn indicator)
4. Castling rights ((K?Q?k?q?)|-) (4 bits)
5. En passant square (4 bits, 1 for bool, 3 for column if true) (use turn indicator for row)

### Pieces on board

1. If contiguous, 4 bits per square, white

## Database

- Look into normal forms 
    - Move redundancy is ok, binary for searching prefix and moves table for 

### Games table

- GameID
- UTC Datetime
- White Username
- Black Username
- White ELO
- Black ELO
- Time Control
- Increment
- Result (win/loss/draw)
- Result category
- Number of (half) moves
- MoveBlob (blob, 2B per half move)

### Moves table

- MoveID
- Color
- GameID (foreign key to Games table)
- PieceType
- From
- To
- Check/Checkmate/Stalemate (nullable?)
- Captured PieceType (nullable)
- Promoted PieceType (nullable)
- Annotation (nullable)

- SecondsRemaining (float, nullable)
- Evaluation (nullable)

- WhiteCanCastleKS (boolean)
- WhiteCanCastleQS (boolean)
- BlackCanCastleKS (boolean)
- BlackCanCastleQS (boolean)
- EnPassant (nullable, 1-8=(file a-h))
- HalfMoveCounter (int)
- FullTurnCounter (int >= 1)
- PositionID (foreign key in Positions table)

### Positions table

- PositionID
- Position (blob, 32B)
    - Ensure empty squares are standardized to 4 bits of 0.

### ECO table

- Entry (PK)
- Code (varchar(3))
- Move binary prefix

## Runtime Interface for Apps (console and GUI)

- State-based main board position and move order
    - Select branch of moves (player created or from DB)
    - Next/prev/jumpto move
    - Delete player created branch (to declutter)
    - Apply move, making new branch or continuing the current one (try matching existing branches?)
- Query database
    - Position
    - Move order prefix
    - Usernames
    - Individual moves? (e.g., b4+ for white, b4+ for either, b8=Q#)
- Best engine moves
- Book moves (ECO table in database)
- Endgame tables (<7 pieces) (another table in database)
