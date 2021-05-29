# PvP-Chess-game
A PvP chess game written in C++ with a few exceptions.

## How it works
Input and Output are displayed on the console, open .exe folder to run the executable version. White pieces are capitalized, black pieces aren't.

## Input format
It uses normal chess Algebraic notation to read the moves, For eg. to move the Pawn to E4 from E2, input format should be in the form of a 4 letter string i.e. "E2E4".

## Exceptions
This code is not reliable to detect draws.
It also cannot detect whether another piece can obstruct check. It instead asks the user to check if its possible and input "yes" if that is the case; Any other input will return false.
EnPassant cannot be performed because previous moves cannot be stored.

## Future Versions 
- Working on the code for the exceptions listed above and will push them as soon as possible.
- Pull requests are welcome. if any bugs are encountered, feel free to fix them.
- Open an issue to discuss major bugs/issues before changes are made.
