# Notes

Intend to release as a minor competitor to ChessBase, open source, pay for installer, etc.

## 2023

- Internal game state, logic, time control
- File formats, primarily for reading and exporting PGN
- Engine interface, uses boost::process to launch and control a UCI-based engine (e.g., Stockfish)
- Database interface with PostGreSQL16 for querying games, openings, etc.
- Scripts for basic testing of features
- Unit and integration tests (TODO)

- TODO: 
    - [x] Make CMakeLists.txt for each dir (recently restructured layout)
    - [x] Bugfix for build success (comment out lines if necessary)
    - [ ] Continue with database interface (leave UCI interface for later)
        - [x] Add insertGame function
        - [ ] Add query positions function
        - [ ] Add query opening function
    - [ ] Add standard game logic, use enums from internal/game headers
        - [ ] Add legal moves logic (see pseudocode below)
            - [x] Add square offset logic for use with legal moves logic
    - [ ] Create JChess class for interfacing

## Week of 2024-01-01

- Changed build system from CMake to premake5
- Reinstalled pqxx
- Restructured directories under src/
- Added database/blobs and database/insert
- Deleted database/database (we'll work directly with pqxx)
- Created database/insert for inserting a Game object into the database (tables "games", "positions", and "moves")
    - TODO: compile and test
    - Note: may want to remove "moves" table and add a "moves" column to the "games" table, each element is an array of Move types
    - Similarly, add columns for arrays of clocks, annotations, evaluations, positionIDs
    - Add rule ensuring that for each row, any of the above columns with non-null values should have the same number of array values
- Started legal move logic; pseudocode:
```python3
def legal_moves(state, color):
    moves = list()

    pos = state.pos

    ksq = get_king_square(pos, color)
    cr = get_castle_rights(color)
    nattack = num_pieces_attacking(ksq, color)
    in_check = nattack >= 1

    for move in all_moves(pos, ksq, cr):
        if (
            not is_attacked(move.to)
            and (
                not is_castle(move)
                or (
                    not in_check(color) 
                    and not is_attacked(mid(move.to, move.from), color) 
                    and all(is_empty(sq) for sq between king and rook)
                )
            )
        ):
            moves.append(move)
    
    if nattack > 1:
        return moves
    
    for sq, piece in enumerate(pos):
        if piece is None or piece.color != turn or piece.type == king:
            continue
        if is_attacked(sq, color):
            hard_pin = get_pin(pos, sq)
        ep_possible = (state.enPassant is not None) and (piece.type == pawn) and (sq is one of two correct)
        for move in all_moves(pos, sq):
            if in_check and not (move blocks or captures attacker)
                continue
            if hard_pin and not (move captures attacker or is along path):
                continue
            if ep_possible:
                check for ksq in rank and enemy rook or queen in rank and only these two pawns between them for each rook, queen in rank
                if taking ep would result in check:
                    continue
            moves.append(move)
```