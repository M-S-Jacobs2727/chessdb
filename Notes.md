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
    - [ ] Add standard game logic, use enums from internal/game headers

## Week of 2024-01-01

- Changed build system from CMake to premake5
- Reinstalled pqxx
- Restructured directories under src/
- Added database/blobs and database/insert
- Deleted database/database (we'll work directly with pqxx)