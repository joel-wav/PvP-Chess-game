# PvP-Chess-game
A PvP chess game written in C++ with a few exceptions.

## How it works
Input and Output are displayed on the console, open .exe folder to run the executable version. White pieces are capitalized, black pieces aren't.

## Input format
It uses normal chess Algebraic notation to read the moves, For eg. to move the Pawn to E4 from E2, input format should be in the form of a 4 letter string i.e. "E2E4".

## Exceptions
This code is not reliable to detect draws and double checks.
It also cannot detect whether a piece that checks the king can be captured, or whether another piece can obstruct the path of check on its own. It instead asks the user to check and if either is possible, input "yes". Any other input will be taken as a no.
EnPassant cannot be performed because previous moves cannot be stored.

## Future Versions 
- plan to add a GUI.
- I'm working on the code for the exceptions listed above and will push them as soon as possible.
- Pull requests are welcome. if any bugs are encountered, feel free to fix them.
- Open an issue to discuss major bugs/issues before changes are made.
