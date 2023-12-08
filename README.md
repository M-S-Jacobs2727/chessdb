# chessdb

## PGN ZST Format

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
17. Termination ("Normal", "Time Forfeit")

### Move Notations
\d+\.(\.\.)? [\w\d\+#]+[?!]+ \{ (\[%eval ((#-?\d+)|(-?\d+\.\d+))\])? (\[%clk \d+:\d\d:\d\d\])? \}

### Result
(0-1)|(1-0)|(1/2-1/2)

## Binary Format
Ints are unsigned unless noted

### Header
1. format ID
2. white username length (16-bit int)
3. black username length (16-bit int)
4. white username
5. black username
6. Binary format UTCDate and time
7. White ELO (16-bit int)
8. Black ELO (16-bit int)
9. ECO code (3 chars)
10. Time control (16-bit int, 16-bit int)
11. Include Evaluation (1 or 8 bit bool)
12. Include Clock (1 or 8 bit bool)

### Move Notations
1. Piece ID (12 pieces -> 4 bits necessary)
2. Start square (6 bits)
3. End square (6 bits)
4. Check/mate/stalemate (2 bits)
5. Annotation (?, !, !!, ??, ?!, !?) (3 bits)
6. Evaluation (optional, if H11) (32-bit float, NaNs signal with 8-bit signed int in significand for mates-in-x)
7. Clock (optional, if H12) (seconds as 16-bit int)
