//testingcommits
#include<stdlib.h>
#include<iostream>
#include<string>
#include<cmath>
#include<vector>
#include<utility>

using namespace std;

#define WHITE_SQUARE char(0xDB) //to print white squares
#define BLACK_SQUARE char(0xFF) //to print black squares

enum Piece { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };
enum Color { WHITE, BLACK, NONE };

class Square
{
	Piece piece;
	Color color;
	int x, y;
public:
    Square()
    { 
		piece = EMPTY ;
        color = NONE;
    }
	
	void setSpace(Square* space)
    {
        color = space->getColor();
        piece = space->getPiece();
    }

    void setEmpty()
    {
        color = NONE;
        piece = EMPTY;
    }

	void setPieceAndColor(Piece p, Color c)
    {
        piece = p;
        color = c;
    }
	
	Piece getPiece() 
	{ 
		return piece; 
	}
	
	Color getColor() 
	{ 
		return color; 
	}
	
	void setX(int ex) 
	{ 
		x = ex; 
	}
	
	void setY(int why) 
	{ 
		y = why; 
	}
	
	int getX() 
	{ 
		return x; 
	}
	
	int getY() 
	{ 
		return y; 
	}
};

class Board
{
	Square square[8][8];
	Color turn = WHITE;

    bool king_is_in_check = false; //this variable is solely to decide checkmate and not to decide 
	                               //the current state of the king (i.e whether it is in check or not)
								   //to check if the king is in check or not, we seperately call the isKingInCheck fn.

	bool short_castles_white = true;
	bool long_castles_white = true;
	bool short_castles_black = true;
	bool long_castles_black = true;

	/*Piece check_piece;
	Color check_color;*/
	int check_x, check_y;
	
	void printBoard()
    {
        int cell_size = 6;
        cout<<"  1     2     3     4     5     6     7     8   "<<endl;
        int index = 1;
        for(int m = 0 ; m < 4 ; m++)
        {
            for(int i = 0 ; i < cell_size/2 ; i++)
            {
                for(int k = 0 ; k < 4 ; k++)
                {
                    for(int j = 0 ; j < cell_size ; j++)
                    {
                        if (i==1 && j==2)
                            printPieces(2*m,2*k,WHITE_SQUARE);
                        else
                            cout<<WHITE_SQUARE;
                    }
                    for(int j = 0 ; j < cell_size ; j++)
                    {
						if (i==1 && j==2)
                            printPieces(2*m,2*k+1,BLACK_SQUARE);
                        else
                            cout<<BLACK_SQUARE;
                    }
                }
                if (i==1)
                {
                    cout<<" "<<char(index+64);
                    index++;
                }
                cout<<endl;
            }
            for(int i = 0 ; i < cell_size/2 ; i++)
            {
                for(int k = 0 ; k < 4 ; k++)
                {
                    for(int j = 0 ; j < cell_size ; j++)
                    {
						if (i==1 && j==2)
                            printPieces(2*m+1,2*k,BLACK_SQUARE);
                        else
                            cout<<BLACK_SQUARE;
                    }
                    for(int j = 0 ; j < cell_size ; j++)
                    {
					    if (i==1 && j==2)
                            printPieces(2*m+1,2*k+1,WHITE_SQUARE);
                        else
                            cout<<WHITE_SQUARE;
                    }
				}
				if (i==1)
                {
                    cout<<" "<<char(index+64);
                    index++;
                }
				cout<<endl;
            }
        }
	}
	
	void printPieces(int i, int j, int color)
    {
	    Piece p = square[i][j].getPiece();
		Color c = square[i][j].getColor();

		switch (p)
		{
			case KING: (c == WHITE) ? cout << "K" : cout << "k"; break;
			case QUEEN: (c == WHITE) ? cout << "Q" : cout << "q"; break;
			case BISHOP: (c == WHITE) ? cout << "B" : cout << "b"; break;
			case KNIGHT: (c == WHITE) ? cout << "N" : cout << "n"; break;
			case ROOK: (c == WHITE) ? cout << "R" : cout << "r"; break;
			case PAWN: (c == WHITE) ? cout << "P" : cout << "p"; break;
			case EMPTY: cout << char(color); break;
			default: cout << "ERROR 101"; break;
		}
    }

public:
	Square* getSquare(int x, int y)
	{
		if (x == -1 || y == -1)
		    return NULL;
		else 
		    return &square[x][y];
	}

	Square* getKingSquare()
	{
		for(int i = 0 ; i < 8 ; i++)
		{
			for(int j = 0 ; j < 8 ; j++)
			{
				if (square[i][j].getPiece() == KING && square[i][j].getColor() == turn)
				{
				    cout<<"\nKing Square is: "<<i<<" "<<j<<endl;
					return getSquare(i,j);
				}
			}
		}
		cout<<"Returning NULL"<<endl;
		return NULL;
	}

	void setSquare(Square * s, int x, int y)
	{
		square[x][y]=*s;
	}

	bool isCheckmate(Square* kingSquare)
	{
		if (!isKingInCheck(kingSquare))
		{
			king_is_in_check = false;
			cout<<turn<<" "<<"king is not in check"<<endl;
		    return false;
		}
		else 
		{
			if (canKingMoveOrKill(kingSquare))
			{
				cout<<"KING CAN MOVE"<<endl;
				cout<<"Check"<<endl;
				return false;
			}
			else if (canPieceBeCaptured(kingSquare))
			{
				king_is_in_check = true;
				cout<<"ATTACKING PIECE CAN BE CAPTURED"<<endl;
				cout<<"Check"<<endl;
				return false;
			}
			else if (canPieceObstruct())
			{
				cout<<"PIECE CAN OBSTRUCT"<<endl;
				cout<<"Check"<<endl;
				return false;
			}
			else 
			{
				cout<<"CheckMate. ";
				if (turn == WHITE)
				    cout<<"Black wins"<<endl;
				else
				    cout<<"White wins"<<endl;
				return true;
			}
		}
	}
	
    bool canKingMoveOrKill(Square* kingSquare)
	{
		Color opp;
		
		if (turn == WHITE)
		    opp = BLACK;
		else 
		    opp = WHITE;

        int x = kingSquare->getX();
		int y = kingSquare->getY();
		int ex, why;

		for(int i = -1 ; i < 2; i++)
		{
			for(int j = -1 ; j < 2 ; j++)
			{
				if (i == 0 && j == 0)
				    continue;
				ex = x + i, why = y + j;
				cout<<ex<<" "<<why<<"   ";
				if (ex < 0 || ex > 7 || why < 0 || why > 7)
				    continue;
				if (square[ex][why].getColor() == turn)
				    continue;
				if (!isSquareUnderThreat(opp,ex,why))
				{
					cout<<"Returning true because that last square is not under threat"<<endl;
					return true;
				}
			}
		}
        return false;
	}

    bool canPieceBeCaptured(Square* kingSquare)
	{
		Color opponent;
		Square* attacking_piece = getSquare(check_x, check_y);

        if (turn == WHITE)
		    opponent = BLACK;
		else 
		    opponent = BLACK;
        
		if (isSquareUnderThreat(turn, check_x, check_y)) // Note that it checks the validity of the captures move also
		    return true;
		
		return false;
	}
	
	bool canPieceObstruct()
	{
		string obstruct;
		cout<<"Can another piece get in the way? "; cin>>obstruct;
		if (obstruct == "yes")
		    return true;
		else 
		    return false;
	}

	bool doMove()
	{
        string move;
	    int x1, x2, y1, y2;
	    bool stop = false;
	    while (!stop)
	    {
		    (turn == WHITE) ? cout<<"White's turn"<<endl : cout<<"Black's turn"<<endl;
		    cout<<"Type in your move [(R/r)esign]: ";
		    cin >> move;
			if (move == "R" || move == "r" )
			{
				turn == (WHITE) ? cout<<"White resigns, Black wins" : cout<<"Black resigns, White wins";
				cout<<endl;
				return false;
			}
			//input is via normal chess algebra (ALL CAPS, i think xD)
		    x1 = move[0] - 65;
		    y1 = move[1] - 49;
		    x2 = move[2] - 65;
		    y2 = move[3] - 49;
		    if (getSquare(x1, y1)->getColor() == turn)
		    {
                if (makeMove(x1, y1, x2, y2) == false)
			        cout<<"Invalid move, try again."<<endl;
			    else
                    stop = true;
            }
		    else
			    cout<<"Not your turn"<< endl;
	    }

	    cout<<"Changing turns"<<endl;

        if (turn == BLACK)
		{
			turn = WHITE;
			Square* kingSquare = getKingSquare();
            if (isCheckmate(kingSquare))
			    return false;
			king_is_in_check = false;
		}
	    else
		{
			turn = BLACK;
			Square* kingSquare = getKingSquare();
            if (isCheckmate(kingSquare))
			    return false;
			king_is_in_check = false;
		}

        return true;
    }

	void setBoard()
    {
		//white main pices
        square[0][0].setPieceAndColor(ROOK, WHITE);
	    square[1][0].setPieceAndColor(KNIGHT, WHITE);
	    square[2][0].setPieceAndColor(BISHOP, WHITE);
	    square[3][0].setPieceAndColor(QUEEN, WHITE);
	    square[4][0].setPieceAndColor(KING, WHITE);
	    square[5][0].setPieceAndColor(BISHOP, WHITE);
	    square[6][0].setPieceAndColor(KNIGHT, WHITE);
	    square[7][0].setPieceAndColor(ROOK, WHITE);
        //black main pieces
        square[0][7].setPieceAndColor(ROOK, BLACK);
	    square[1][7].setPieceAndColor(KNIGHT, BLACK);
	    square[2][7].setPieceAndColor(BISHOP, BLACK);
	    square[3][7].setPieceAndColor(QUEEN, BLACK);
	    square[4][7].setPieceAndColor(KING, BLACK);
	    square[5][7].setPieceAndColor(BISHOP, BLACK);
	    square[6][7].setPieceAndColor(KNIGHT, BLACK);
	    square[7][7].setPieceAndColor(ROOK, BLACK);
	    //both black and white pawns
		for (int i = 0; i < 8; i++)
	    {
	 	    square[i][1].setPieceAndColor(PAWN, WHITE);
		    square[i][6].setPieceAndColor(PAWN, BLACK);
        }
        //set other squares as empty with no color
	    for (int i = 2; i < 6; i++)
	    {
		    for (int j = 0; j < 8; j++)
			    square[j][i].setPieceAndColor(EMPTY, NONE);

	    }

	    for (int i = 0; i < 8; i++)
	    {
            for (int j = 0; j < 8; j++)
		    {
			    square[i][j].setX(i);
			    square[i][j].setY(j);
		    }
        }
    }

	bool playGame(string player_white, string player_black)
    {
		//clear the screen and output the board after every new move
		//top left shows name of white player
		//bottom right shows name of black player
        //system("CLS");
		cout <<"Chess v1.0"<<endl<<"by Joel Jacob"<<endl<<endl;
		cout<<player_white<<endl;
        printBoard();
		cout<<"\t\t\t\t\t    "<<player_black<<endl;
        return doMove();
        return false;//not necessary
    }

    bool makeMove(int x1, int y1, int x2, int y2)
	{
		if (x1 < 0 || x1>7 || y1 < 0 || y1>7 || x2 < 0 || x2>7 || y2 < 0 || y2>8)
	    {
		    std::cout << "out of bounds" << std::endl;
		    return false;
	    }
	    Square* src = getSquare(x1, y1);
	    Square* dest = getSquare(x2, y2);

	    if (src->getColor() == dest->getColor() && dest->getColor() != NONE)
	    {
		    cout << "Cannot land on your own piece" << std::endl;
		    return false;
	    }

	    switch (src->getPiece())
	    {
	        case KING: return moveKing(src, dest); break;
	        case QUEEN: return moveQueen(src, dest); break;
	        case BISHOP: return moveBishop(src, dest); break;
	        case KNIGHT: return moveKnight(src, dest); break;
	        case ROOK: return moveRook(src, dest); break;
	        case PAWN: return movePawn(src, dest); break;
	        case EMPTY: std::cout<<"You do not have a piece there"<<std::endl;  return false; break;
	        default: cout<<"ERROR 101: switch"<<std::endl; break;
	    }
	    return false;
	}

    bool isKingInCheck(Square* kingSquare)
	{
		int ex = kingSquare->getX();
		int why = kingSquare->getY();
		Color opp;
		if (turn == WHITE)
		   opp = BLACK;
		else if (turn == BLACK)
		   opp = WHITE;
		//cout<<turn<<" "<<opp<<endl;
		return isSquareUnderThreat(opp,ex,why) ;
		
	}

    bool validateShortCastle(Square* kingSquare)
	{//the condition that all the squares in the way should be empty is not checked here
        int ex, why;
		Color opp;
		ex = kingSquare->getX();
		why = kingSquare->getY();
		if (turn == WHITE)
            opp = BLACK;
		else
		    opp = WHITE;
		if (isKingInCheck(kingSquare) || isSquareUnderThreat(opp,ex+1,why) || isSquareUnderThreat(opp,ex+2,why))
		    return false;
		if (turn == WHITE)
		    return short_castles_white;
        else
		    return short_castles_black;
	}

	bool validateLongCastle(Square* kingSquare)
	{//the condition that all the squares in the way should be empty is not checked here, add only if necessary
		int ex, why;
		Color opp;
		ex = kingSquare->getX();
		why = kingSquare->getY();
		turn == WHITE? opp = BLACK : opp = WHITE;

		if ( isKingInCheck(kingSquare) || isSquareUnderThreat(opp,ex-1,why) || isSquareUnderThreat(opp,ex-2,why))
		    return false;

		if (turn == WHITE)
		    return long_castles_white;
        else
		    return long_castles_black;
	}

	bool isSquareUnderThreat(Color opponent, int x, int y)
	{
		Square *candidate_square;
		int ex, why;
		Color temp;

		//cout<<opponent<<endl;

		if (opponent == WHITE)
		    temp = BLACK;
		else 
		    temp = WHITE;
        
		//cout<<temp<<" "<<opponent<<endl;
		//check diagonal (for bishop and queen)
		//forward up
		cout<<"CHECKING DIAGONALS"<<endl;
		ex = x, why = y;
		//cout<<"d1 ";
		while(true)
		{
			ex = ex + 1;
			why = why + 1;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			{
			    break;
			}
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
            {
                break;
            }
			if ((candidate_piece == BISHOP || candidate_piece == QUEEN) && candidate_color == opponent)
            {
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				//cout<<check_x<<" "<<check_y<<endl;
                return true;
            }
		}
		//backward up
		//cout<<"d2 ";
		ex = x, why = y;
		while(true)
		{
			ex = ex - 1;
			why = why + 1;
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			{
			    break;
			}
			candidate_square = getSquare(ex, why);
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == BISHOP || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				//cout<<check_x<<" "<<check_y<<endl;
                return true;
            }
		}
		//backward down
		//cout<<"d3 ";
		ex = x, why = y;
		while(true)
		{
			ex = ex + 1;
			why = why - 1;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			{
			    break;
			}
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == BISHOP || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				//cout<<check_x<<" "<<check_y<<endl;
                return true;
            }
		}
		//forward down
		//cout<<"d4 ";
		ex = x, why = y;
		while(true)
		{
			ex = ex - 1;
			why = why - 1;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			{
			    break;
			}
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == BISHOP || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
		    	//cout<<check_x<<" "<<check_y<<endl;
                return true;
            }
		}

		//check vertical (for rook and queen)
		//up
		cout<<"CHECKING VERT AND HOR"<<endl;
		ex = x, why = y;
		while(true)
		{
			ex = ex;
			why = why + 1;
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			    break;
            candidate_square = getSquare(ex, why);
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == ROOK || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
			    return true;
			}
		}
		//down
		ex = x, why = y;
		while(true)
		{
			ex = ex;
			why = why - 1;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			    break;
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == ROOK || candidate_piece == QUEEN) && candidate_color == opponent)
		    {
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				return true;
			}
		}

		//check horizontal (for rook and queen)
		//left
		ex = x, why = y;
		while(true)
		{
			ex = ex + 1;
			why = why;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			    break;
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == ROOK || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				return true;
			}
		}
		//right
		ex = x, why = y;
		while(true)
		{
			ex = ex;
			why = why - 1;
			candidate_square = getSquare(ex, why);
			if (ex < 0 || ex > 7 || why < 0 || why > 7)
			    break;
			Piece candidate_piece = candidate_square->getPiece();
			Color candidate_color = candidate_square->getColor();
			if (candidate_color == temp)
			{
                break;
            }
			if ((candidate_piece == ROOK || candidate_piece == QUEEN) && candidate_color == opponent)
			{
				/*check_piece = candidate_piece;
				check_color = candidate_color;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
					else 
					    break;
				}
				check_x = ex;
				check_y = why;
				return true;
			}
		}

        cout<<"CHECKING KNIGHTS"<<endl;
		//check L (for Knights)
		// all 8 possible squares
		ex = x + 2, why = y + 1;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x + 2, why = y - 1;
        if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x - 2, why = y + 1;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x - 2, why = y - 1;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x + 1, why = y + 2;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x + 1, why = y - 2;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
        ex = x - 1, why = y + 2;
		if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}
		ex = x - 1, why = y - 2;
		if (ex >= 0 && ex <= 7 && why >= 0 && why <= 7)
		{
			candidate_square = getSquare(ex, why);
		    if (candidate_square->getPiece() == KNIGHT && candidate_square->getColor() == opponent)
		    {
				/*check_piece = KNIGHT;
				check_color = opponent;*/
				if (king_is_in_check == true)
				{
					if (checkValidity(ex,why))
					    return true;
				}
				else 
				{
					check_x = ex;
				    check_y = why;
				    return true;
				}
			}
		}

        cout<<"CHECKING PAWNS"<<endl;
		//check one step diagonal (for pawns)
		if (temp == WHITE)//black pawns attacking
        {
			//2 cases
			ex = x - 1, why = y + 1;
			if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
			{
				//cout<<"Temp is white"<<endl;
				Square* candidate_square = getSquare(ex, why);
			    if (candidate_square->getPiece() == PAWN && candidate_square->getColor() == opponent)
		        {
					//cout<<"true"<<endl;
				    /*check_piece = PAWN;
				    check_color = opponent;*/
					if (king_is_in_check == true)
				    {
					    if (checkValidity(ex,why))
					        return true;
				    }
					else 
					{
						check_x = ex;
				        check_y = why;
				        return true;
					}
			    }
			}
			ex = x + 1, why = y + 1;
			if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
			{
				Square* candidate_square = getSquare(ex, why);
			    if (candidate_square->getPiece() == PAWN && candidate_square->getColor() == opponent)
		        {
				    /*check_piece = PAWN;
				    check_color = opponent;*/
					if (king_is_in_check == true)
				    {
					    if (checkValidity(ex,why))
					        return true;
			 	    } 
					else 
					{
						check_x = ex;
				        check_y = why;
				        return true;
					}
			    }
			}
		}
		else if (temp == BLACK) //white pawns attacking
		{   //cout<<"temp is black"<<endl;
			//2 cases
			ex = x + 1, why = y - 1;
			if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
			{
				Square* candidate_square = getSquare(ex, why);
			    if (candidate_square->getPiece() == PAWN && candidate_square->getColor() == opponent)
		        {
				    /*check_piece = PAWN;
				    check_color = opponent;*/
					if (king_is_in_check == true)
				    {
					    if (checkValidity(ex,why))
					        return true;
			      	}
					else 
					{
						check_x = ex;
				        check_y = why;
				        return true;
					}
			    }
			}
			ex = x - 1, why = y - 1;
			if (ex >=0 && ex <= 7 && why >= 0 && why <= 7)
			{
				Square* candidate_square = getSquare(ex, why);
			    if (candidate_square->getPiece() == PAWN && candidate_square->getColor() == opponent)
		        {
				    /*check_piece = PAWN;
				    check_color = opponent;*/
					if (king_is_in_check == true)
				    {
					    if (checkValidity(ex,why))
					        return true;
			      	}
					else 
					{
						check_x = ex;
				        check_y = why;
				        return true;
					}
			    }
			}
		}

        cout<<"CHECKING KING"<<endl;
		//all surrounding squares by one step (for king)
		for(int i = -1 ; i < 2 ; i++)
		{
			for(int j = -1 ; j < 2 ; j++)
			{
				if (i==0 && j==0)
				    continue;
				ex = x + i, why = y + j;
				if (ex < 0 || ex > 7 || why < 0 || why > 7)
                    continue;
				//cout<<ex<<" "<<why<<endl;
                candidate_square = getSquare(ex,why);
				if (candidate_square->getPiece() == KING && candidate_square->getColor() == opponent)
				{
				    /*check_piece = KING;
				    check_color = opponent;*/
					if (king_is_in_check == true)
				    {
					    if (checkValidity(ex,why))
					        return true;
					}
					else 
					{
					    check_x = ex;
				        check_y = why;
				        return true;
					}
			    }
            }
		}
		//if none of the pieces from opposite color are attacking, square is not under threat
		/*check_piece = EMPTY;
		check_color = NONE;*/
		if (king_is_in_check == true)
		    return false;
		check_x = -1;
		check_y = -1;
		return false;
    }
    
    bool checkValidity(int x, int y)
	{
		Square* that_space = getSquare(check_x, check_y);
		Square* this_piece = getSquare(x,y);
        Color temp_color;
		Piece temp_piece;
        
	    temp_color = that_space->getColor();
		temp_piece = that_space->getPiece();
		
        that_space->setSpace(this_piece);
		this_piece->setEmpty();

		Square* kingSquare = getKingSquare();

		bool check = isKingInCheck(kingSquare);
		
		this_piece->setSpace(that_space);
		that_space->setPieceAndColor(temp_piece, temp_color);

	    return check;
	}

   //Piece moves

	bool moveKing(Square* thisKing, Square* thatSpace)
	{
		if (abs(thatSpace->getX() - thisKing->getX()) == 1 || abs(thatSpace->getY() - thisKing->getY()) == 1)
		{
			thatSpace->setSpace(thisKing);
			thisKing->setEmpty();
			if (!isKingInCheck(thisKing))
			{
				if (thatSpace->getColor() == WHITE) //alternatively if (turn == WHITE)
			    {
					short_castles_white = false;
			        long_castles_white = false;
				}
				else
                {
					short_castles_black = false;
					long_castles_black = false;
				}
				return true;
			}
			else
			{
				thisKing->setSpace(thatSpace);
				thatSpace->setEmpty();
				return false;
			}
		}
		else if (thatSpace->getX() - thisKing->getX() == 2 && validateShortCastle(thisKing)) //for short castling
		{
			thatSpace->setSpace(thisKing);
			thisKing->setEmpty();
			Square* here = getSquare(7, thatSpace->getY());
			Square* there = getSquare(5, thatSpace->getY());
			there->setSpace(here);
			here->setEmpty();
			if (turn == WHITE)
			{
				short_castles_white = false;
			    long_castles_white = false;
			}
			else
			{
				short_castles_black = false;
				long_castles_black = false;
			}
			return true;
		}
		else if (thatSpace->getX() - thisKing->getX() == -2 && validateLongCastle(thisKing)) //for long castling
		{
			thatSpace->setSpace(thisKing);
			thisKing->setEmpty();
			Square* here = getSquare(0, thatSpace->getY());
			Square* there = getSquare(3, thatSpace->getY());
			there->setSpace(here);
			here->setEmpty();
			if (turn == WHITE)
			{
				short_castles_white = false;
			    long_castles_white = false;
			}
			else
			{
				short_castles_black = false;
				long_castles_black = false;
			}
			return true;
	    }
	    else
	        return false;
    }

    bool moveQueen(Square* thisQueen, Square* thatSpace)
    {
		int queenX = thisQueen->getX();
	    int queenY = thisQueen->getY();
	    int thatX = thatSpace->getX();
	    int thatY = thatSpace->getY();
	    int yIncrement;
	    int xIncrement;

	    bool invalid = false;
	    if (queenX != thatX || queenY != thatY)
	    {
			if (queenX == thatX)
		    {
			    yIncrement = (thatY - queenY) / (abs(thatY - queenY));
			    for (int i = queenY + yIncrement; i != thatY; i += yIncrement)
			    {

				    if (square[thatX][i].getColor() != NONE)
					    return false;

			    }
		    }
		    else if (queenY == thatY)
			{
				xIncrement = (thatX - queenX) / (abs(thatX - queenX));
			    for (int i = queenX + xIncrement; i != thatX; i += xIncrement)
			    {
				    if (square[i][thatY].getColor() != NONE)
					    return false;
			    }
			}
			else if (abs(queenX - thatX) == abs(queenY - thatY))
			{
			    xIncrement = (thatX - queenX) / (abs(thatX - queenX));
			    yIncrement = (thatY - queenY) / (abs(thatY - queenY));
			    for (int i = 1; i < abs(queenX - thatX); i++)
			    {
				    //cout << "It got here somehow";
				    if (square[queenX + xIncrement*i][queenY + yIncrement*i].getColor() != NONE)
					    return false;
                }
			}
			else
				return false;
	    }
        if (!invalid)
	    {
		    thatSpace->setSpace(thisQueen);
		    thisQueen->setEmpty();
			Square* kingSquare = getKingSquare();
			if (!isKingInCheck(kingSquare))
			{
				//cout<<"Queen Move, King is not in Check"<<endl;
				return true;
			}
		    else
			{
				thisQueen->setSpace(thatSpace);
				thatSpace->setEmpty();
			    return false;
			}
	    }
	    else
		    return false;
    }

    bool moveBishop(Square* thisBishop, Square* thatSpace)
	{
		int bishopX = thisBishop->getX();
	    int bishopY = thisBishop->getY();
	    int thatX = thatSpace->getX();
	    int thatY = thatSpace->getY();
	    bool invalid = false;
	    if (abs(bishopX - thatX) == abs(bishopY - thatY))
	    {
		    int dx = (thatX - bishopX) / (abs(thatX - bishopX));
		    int dy = (thatY - bishopY) / (abs(thatY - bishopY));

		    for (int i = 1; i < abs(bishopX - thatX); i++)
		    {
			    //cout << "It got here somehow";
			    if (square[bishopX + dx*i][bishopY + dy*i].getColor() != NONE)
				    return false;
			}
	    }
	    else
		    return false;
		if (!invalid)
	    {
		    thatSpace->setSpace(thisBishop);
		    thisBishop->setEmpty();
			Square* kingSquare = getKingSquare();
			if (!isKingInCheck(kingSquare))
				return true;
		    else
			{
				thisBishop->setSpace(thatSpace);
				thatSpace->setEmpty();
			    return false;
			}
	    }
	    else
		    return false;
    }

    bool moveKnight(Square* thisKnight, Square* thatSpace)
    {
		int knightX = thisKnight->getX();
	    int knightY = thisKnight->getY();
	    int thatX = thatSpace->getX();
	    int thatY = thatSpace->getY();

	    if ((abs(knightX - thatX) == 2 && abs(knightY - thatY) == 1) || (abs(knightX - thatX) == 1 && abs(knightY - thatY) == 2))
	    {
			thatSpace->setSpace(thisKnight);
		    thisKnight->setEmpty();
			Square* kingSquare = getKingSquare();
		    if (!isKingInCheck(kingSquare))
				return true;
		    else
			{
				thisKnight->setSpace(thatSpace);
		        thatSpace->setEmpty();
			    return false;
			}
	    }
	    else
	        return false;
    }

    bool moveRook(Square* thisRook, Square* thatSpace)
    {
		int rookX = thisRook->getX();
	    int rookY = thisRook->getY();
	    int thatX = thatSpace->getX();
	    int thatY = thatSpace->getY();
	    bool invalid = false;
	    if (rookX != thatX || rookY != thatY)
	    {
		    if (rookX == thatX)
		    {
			    int yIncrement = (thatY - rookY) / (abs(thatY - rookY));
			    for (int i = rookY + yIncrement; i != thatY; i += yIncrement)
			    {
					if (square[thatX][i].getColor() != NONE)
					    return false;

			    }
		    }
		    else
			    if (rookY == thatY)
			    {
					int xIncrement = (thatX - rookX) / (abs(thatX - rookX));
				    for (int i = rookX + xIncrement; i != thatX; i += xIncrement)
				    {
					    if (square[i][thatY].getColor() != NONE)
						    return false;
				    }
			    }
			    else
				    return false;
	    }
	    if (!invalid)
	    {
		    thatSpace->setSpace(thisRook);
		    thisRook->setEmpty();
			Square* kingSquare = getKingSquare();
			if (!isKingInCheck(kingSquare))
				return true;
		    else
			{
				thisRook->setSpace(thatSpace);
				thatSpace->setEmpty();
			    return false;
			}
	    }
	    else
		    return false;
    }

    bool movePawn(Square* thisPawn, Square* thatSpace)
    {
		//cout<<"falnse"<<endl;
	    bool invalid = false;
	    int pawnX = thisPawn->getX();
	    int pawnY = thisPawn->getY();
	    int thatX = thatSpace->getX();
	    int thatY = thatSpace->getY();

     	if (thisPawn->getColor() == WHITE) //for white pawn; single and double moves
	    {
			//cout<<"\nHello";
			if (pawnX == thatX && (thatY == pawnY + 1 || (pawnY == 1 && thatY == pawnY + 2)) && thatSpace->getColor() == NONE)
		    {
				char ch = 'E';
				if (thatY == 7) //white pawn promotion
			    {
					cout<<"Which Piece do you want to Promote to(Q,R,B,N): ";
					cin>>ch;
					
				}
			    thatSpace->setSpace(thisPawn);
			    thisPawn->setEmpty();
				Square* kingSquare = getKingSquare();
				if (!isKingInCheck(kingSquare))
                {
                    //cout<<"King is not in check"<<endl;
					if (ch != 'E')
					{
						switch(ch)
					    {
						    case 'Q': thatSpace->setPieceAndColor(QUEEN, WHITE);  break;
			                case 'B': thatSpace->setPieceAndColor(BISHOP, WHITE); break;
			                case 'N': thatSpace->setPieceAndColor(KNIGHT, WHITE); break;
			                case 'R': thatSpace->setPieceAndColor(ROOK, WHITE); break;
			                default: cout << "Can't promote to that piece"<<endl; break;
					    }
					}
					
                    return true;
                }
			    else
				{
				    cout<<"Hello, King is in check"<<endl;
					thisPawn->setSpace(thatSpace);
			        thatSpace->setEmpty();
				    return false;
				}
		    }
		    else if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY + 1 == thatY  && thatSpace->getColor() == BLACK) //for white pawn takes
			{
				thatSpace->setSpace(thisPawn);
			    thisPawn->setEmpty();
				Square* kingSquare = getKingSquare();
				if (!isKingInCheck(kingSquare))
                    return true;
                else
				{
				    cout<<"King is in check"<<endl;
					thisPawn->setSpace(thatSpace);
			        thatSpace->setEmpty();
				    return false;
				}
			}
			else
				return false;
	    }
	    else if (thisPawn->getColor() == BLACK) //for black pawn; single and double moves
		{
			char ch = 'E';
			if (thatY == 0) //black pawn promotion
			{
				cout<<"Which Piece do you want to Promote to(q,r,b,n): ";
				cin>>ch;
			}
			if (pawnX == thatX && (thatY == pawnY - 1 || (pawnY == 6 && thatY == pawnY - 2)) && thatSpace->getColor() == NONE)
			{
				thatSpace->setSpace(thisPawn);
			    thisPawn->setEmpty();
				Square* kingSquare = getKingSquare();
				if (!isKingInCheck(kingSquare))
                {
                    //cout<<"King is not in check"<<endl;
					if (ch != 'E')
					{
						switch(ch)
					    {
						    case 'q': thatSpace->setPieceAndColor(QUEEN, BLACK);  break;
			                case 'b': thatSpace->setPieceAndColor(BISHOP, BLACK); break;
			                case 'n': thatSpace->setPieceAndColor(KNIGHT, BLACK); break;
			                case 'r': thatSpace->setPieceAndColor(ROOK, BLACK); break;
			                default: cout << "Can't promote to that piece"<<endl; break;
					    }
					}
                    return true;
                }
			    else
				{
				    cout<<"King is in check"<<endl;
					thisPawn->setSpace(thatSpace);
			        thatSpace->setEmpty();
				    return false;
				}
			}
			else if ((pawnX + 1 == thatX || pawnX - 1 == thatX) && pawnY - 1 == thatY  && thatSpace->getColor() == WHITE) //for black pawn takes
			{
				thatSpace->setSpace(thisPawn);
			    thisPawn->setEmpty();
				Square* kingSquare = getKingSquare();
				if (!isKingInCheck(kingSquare))
					return true;
			    else
				{
				    cout<<"King is in check"<<endl;
					thisPawn->setSpace(thatSpace);
			        thatSpace->setEmpty();
				    return false;
				}
			}
			else
				return false;
		}
		else
			return false;
    }

};

int main()
{
	Board b;
	string s, p1, p2;
	bool newgame = true;
	cout <<"Chess v1.1"<<endl<<"by Joel Jacob"<<endl;
	cout << "Enter any key to continue" << endl;
	cin >> s;

	do
	{
		b.setBoard();
		cout<<"Enter name of Player 1(White): "; cin>>p1; cout<<endl;
		cout<<"Enter name of Player 2(Black): "; cin>>p2;
		while (b.playGame(p1,p2));
		cout << "Do you want to play again? ";
		cin >> s;
	} while(s=="Y"||s=="y");

	return 0;
}


/*
Procedure after any move is played

For eg. If its white's move,

after white plays a move, check whether white's king is in check -- this is checked after every move in their respective functions 
after that, change turns to black 
then check if black king is in checkmate 

*/