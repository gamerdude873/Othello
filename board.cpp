// Authors: Aaron Marsh, Garret Sparks
// File: board.cpp
// Last Updated: 3/29/13
// Description: Board for othello class implementation

/*===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.

Aaron, 3/30: Needed more spacing to read (Sorry garret), commented, Altered piece operations to piece class functions.

Aaron, 4/1: Fixed a serious bug in the cloning algorithm.
====================================================================*/

#include <iostream>
#include "board.h"
#include "piece.h"

// #define DEBUG

using namespace std;

Board::Board()
 {
    //construct a board of no size.
    mSize = 0;
    blackScore = 0;
    whiteScore = 0;
}

Board::Board(int size)
{
    // Construct a board of a certain size

    mSize = size;
    blackScore = 0;
    whiteScore = 0;

    // loop not necessary since each piece is initialized as 'O'

    for(int i = 0; i <= mSize; i++)
    {
        for (int k = 0; k <= mSize; k++)
        {
            mPiecesList[i][k].setColor('O');
        }
    }
}

Board::~Board()
{
    // Deconstructor.
}

Board& Board::clone(Board & rhsBoard) {

	mSize = rhsBoard.getSize();
	whiteScore = rhsBoard.getWhiteScore();
	blackScore = rhsBoard.getBlackScore();

    for(int i = 0; i <= mSize; i++) {
        for(int k = 0; k <= mSize; k++) {
            mPiecesList[i][k] = rhsBoard.mPiecesList[i][k];
        }
    }

	#ifdef DEBUG

	cout << "Original BOard: " << endl;

	rhsBoard.printBoard();

	cout << "Cloned Board: " << endl;

	this -> printBoard();

	#endif


    return *this;
}

char Board::getColorAtLocation(int xCoordinate, int yCoordinate)
{
    return mPiecesList[xCoordinate][yCoordinate].getColor();
}

void Board::placePiece(char color, char xCoordinate, char yCoordinate)
{
    // Place a piece on a blank spot on the board.
    #ifdef DEBUG
    cout << "DEBUG: placing piece at "<< xCoordinate << ","<< yCoordinate << endl;
    cout << "DEBUG: Converting X Coord from ascii to int"<< endl;
    #endif // DEBUG

    int xIntCoord = convertXCoord(xCoordinate);

    #ifdef DEBUG
    cout << "DEBUG: Converting Y Coord from ascii to int"<< endl;
    #endif // DEBUG

    int yIntCoord = convertYCoord(yCoordinate);

    #ifdef DEBUG
    cout << "DEBUG: Coordinates converted to " << xIntCoord << ", " << yIntCoord << endl;
    #endif // DEBUG

	 setColorAtLocation(color, xIntCoord, yIntCoord);
}

int Board::calcHeuristic(char playerColor, int xCoord, int yCoord)
{
	int heuristicVal = 0;
	int scoreValue = 0;
	int terrainValue = 0;
	int oppValue = 0;
	int mobility = 0;
	int x;
	int y;


	// The score difference is important. Also checks for no pieces
	if(playerColor == 'B')
	{
 		scoreValue = (blackScore - whiteScore);
		
		if(blackScore == 0)
		{
			scoreValue = -1000;
		}
		
		else if(whiteScore == 0)
		{
			scoreValue = 20;
		}
	}

	else
	{
 		scoreValue = (whiteScore - blackScore);

		if(whiteScore == 0)
		{
			scoreValue = -1000;
		}

		else if(blackScore == 0)
		{
			scoreValue = 20;
		}
	}


	//Mobility
	for(x = 0; x != 7; x++)
	{
		for(y = 0; y != 7; y++)
		{
			if((getColorAtLocation(x,y) == 'O') && lookAround(x, y, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)))
			{
				mobility -= 5;
			}
			
			if((getColorAtLocation(x,y) == 'O') && lookAround(x, y, playerColor , false, playerColor))
			{
				mobility += 5;
			}
		}
	}

	// These are the base spot values, describing the ostensible value of the spot in a blank board.
	//prioritize corners...

	if(((xCoord == 0) && (yCoord == 0)) ||
		((xCoord == 0) && (yCoord == 7)) ||
		((xCoord == 7) && (yCoord == 7)) ||
		((xCoord == 7) && (yCoord == 0)))
	{
		terrainValue = 25;
	}

	//Edges are good too.

	else if(((xCoord > 1) && (xCoord < 6) && (yCoord == 0)) ||
		((xCoord > 1) && (xCoord < 6) && (yCoord == 7)) ||
		((yCoord > 1) && (yCoord < 6) && (xCoord == 0)) ||
		((yCoord > 1) && (yCoord < 6) && (xCoord == 7))
		)
	{
		terrainValue = 15;
	}

	// avoid spots adjacent to corners.
	else if(((xCoord == 1) && (yCoord == 0)) ||
			((xCoord == 0) && (yCoord == 1)) ||
			((xCoord == 1) && (yCoord == 1)) ||

			((xCoord == 6) && (yCoord == 0)) ||
			((xCoord == 7) && (yCoord == 1)) ||
			((xCoord == 6) && (yCoord == 1)) ||

			((xCoord == 0) && (yCoord == 6)) ||
			((xCoord == 1) && (yCoord == 6)) ||
			((xCoord == 1) && (yCoord == 7)) ||

			((xCoord == 6) && (yCoord == 6)) ||
			((xCoord == 6) && (yCoord == 7)) ||
			((xCoord == 7) && (yCoord == 6)))
	{
		terrainValue = -20;
	}

	// spots next to edges are sorta bad too.
	else
	{
		for (x = 2; x != 5; x++)
		{
			if((xCoord == x) && ((yCoord == 1) || (yCoord == 6)))
			{
				terrainValue = -15;
			}
		}

		for (y = 2; y != 5; y++)
		{
			if((yCoord == y) && ((xCoord == 1) || (xCoord == 6)))
			{
				terrainValue = -15;
			}
		}
	}

	// we really want to pay attention also to what the opponent might be able to do on the next move.
	// These check for the opponent going to edges and corners.

	for(x = 1; x != 6; x++)
	{
		if(((getColorAtLocation(x, 0) == 'O') || (getColorAtLocation(x, 7) == 'O')) &&
			(lookAround(x, 0, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)) ||
			lookAround(x, 7, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor))))
		{
			oppValue += -10;
	
			//Spots next to corner are not as bad to push the opponent to.
			if((x == 1 || x == 6))
			{
				oppValue += 5;
			}
		}
	}

	for(y = 1; y != 6; y++)
	{
		if(((getColorAtLocation(0, y) == 'O') || (getColorAtLocation(7, y) == 'O')) &&
			(lookAround(0, y, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)) ||
			lookAround(7, y, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor))))
		{
			oppValue += -10;
			
			//Spots next to corner are not as bad to push the opponent to.
			if((y == 1 || y == 6))
			{
				oppValue += 5;
			}
		}
	}

	// don't let the opponent get corners
	if((getColorAtLocation(0, 0) == 'O') && lookAround(0, 0, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)))
	{
		oppValue += -25;
	}
	
	if((getColorAtLocation(0, 7) == 'O') && lookAround(0, 7, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)))
	{
		oppValue += -25;
	}

	if((getColorAtLocation(7, 0) == 'O') && lookAround(7, 0, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)))
	{
		oppValue += -25;
	}

	if((getColorAtLocation(7, 7) == 'O') && lookAround(7, 7, getOtherPlayer(playerColor) , false, getOtherPlayer(playerColor)))
	{
		oppValue += -25;
	}

	heuristicVal = (scoreValue + terrainValue + oppValue + mobility);

	return heuristicVal;
}

void Board::setColorAtLocation(char color, int xIntCoord, int yIntCoord)
{
    //Manually change a slot to empty, white or black.

    bool success = false;
    success = mPiecesList[xIntCoord][yIntCoord].setColor(color);

    #ifdef DEBUG
    cout << "DEBUG: Success: " << success << ", Color: "<< mPiecesList[xIntCoord][yIntCoord].getColor() << " at "<< xIntCoord << ", " << yIntCoord<<  endl;
    #endif // DEBUG

    while(!success)   // Set the state, but make sure it's good first
    {
            cout << "Not a valid piece, try again! Input Color (B, W, O):" << endl;
            cin >> color;
            success = mPiecesList[xIntCoord][yIntCoord].setColor(color);
    }

    // updateBoard();
}

void Board::calcScore()
{
	blackScore = 0;
	whiteScore = 0;

	for(int i = 0; i <= mSize; i++)
	{
		for (int k = 0; k <= mSize; k++)
		{
			if (mPiecesList[i][k].getColor() == 'B')
			{
				blackScore++;
			}

			else if (mPiecesList[i][k].getColor() == 'W')
			{
				whiteScore++;
			}
		}
	}
}

bool Board::updateBoard()
{
    // Calculates new score of each player
    bool boardFull = true;
    bool endGame = false;
    setBlackHasMove(false);
    setWhiteHasMove(false);

    #ifdef DEBUG
    cout << "Updating Board!" << endl;
    #endif // DEBUG


	calcScore();

    for(int i = 0; i <= mSize; i++)
    {
       for (int k = 0; k <= mSize; k++)
       {
            if (boardFull && mPiecesList[i][k].getColor() == 'O')
            {
                #ifdef DEBUG
                cout << "DEBUG: Board is not full" << endl;
                #endif // DEBUG

                boardFull = false;
            }

            if(!blackMoveExists && lookAround(i,k, 'B', false, 'O'))
            {
                setBlackHasMove(true);
                #ifdef DEBUG
                cout << "DEBUG: Black has a move" << endl;
                #endif // DEBUG

            }

            if(!whiteMoveExists && lookAround(i,k, 'W', false, 'O'))
            {
                setWhiteHasMove(true);
                #ifdef DEBUG
                cout << "DEBUG: White has a move" << endl;
                #endif // DEBUG
            }

        } // inner loop end
    } // outer loop end


    endGame = (boardFull || blackScore == 0 || whiteScore == 0 ||!(blackMoveExists || whiteMoveExists));

    #ifdef DEBUG
    cout << "DEBUG: endgame is " << endGame << endl;
    #endif // DEBUG

    if(endGame)
    {
            cout << "System: The game is over." << endl;


	if(!(blackMoveExists || whiteMoveExists))
	{
		cout << "System: No moves exist on the board for either player. " << endl;
	}

	if(whiteScore == blackScore)
	{
		cout << "System: We have a tie! Good game." << endl;
	}

	else if(blackScore > whiteScore)
	{
	  	cout << "System: Black player wins! Good game." <<  endl;
	}

	else if(blackScore < whiteScore)
	{
		cout << "System: White player wins! Good game." << endl;
	}

    }

    return !endGame;
}

void Board::printBoard()
{
    // Cout the board.
    cout << "   A B C D E F G H" << endl;
    cout << "   _______________" << endl;
    for(int k = 0; k <= 7; k++)
    {
        cout << k + 1 << "| ";
        for(int i = 0; i <= 7; i++)
        {
            cout << getColorAtLocation(i,k) << " ";
//            cout << i << "," << k << " ";
        }
        cout << endl;
    }

    cout << endl << "Black: " << blackScore << endl;
    cout << "White: " << whiteScore << endl;
}

int Board::convertXCoord(char xCoord)
{
     // convert ascii to int array index Y coordinate is already int, but we need to convert to array indexing
    xCoord = tolower(xCoord);                                            // need lowercase
    return xCoord - 97;
}

int Board::convertYCoord(char yCoord)
{
    return yCoord - 49;
}

bool Board::blackHasMove()
{
    return blackMoveExists;
}

bool Board::whiteHasMove()
{
    return whiteMoveExists;
}

void Board::setBlackHasMove(bool blackCanMove)
{
    blackMoveExists = blackCanMove;
}

void Board::setWhiteHasMove(bool whiteCanMove)
{
    whiteMoveExists = whiteCanMove;
}

bool Board::lookAround(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    bool checkIsValid = false;

        #ifdef DEBUG
        if (currentPlayer == 'B')
            cout << "DEBUG: Looking for Black move..." << endl;

        else
            cout << "DEBUG: Looking for White move..." << endl;

        #endif // DEBUG


	if(getColorAtLocation(xMove, yMove) != 'O')
	{
		return checkIsValid;
	}

    if((inBounds(xMove, yMove+1) &&
        getColorAtLocation(xMove, yMove+1) != currentPlayer &&
        getColorAtLocation(xMove, yMove+1) != 'O') &&
        lookNorth(xMove, yMove + 1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove+1, yMove+1) &&
        getColorAtLocation(xMove+1, yMove+1) != currentPlayer &&
        getColorAtLocation(xMove+1, yMove+1) != 'O') &&
        lookNE(xMove +1, yMove + 1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove+1, yMove) &&
        getColorAtLocation(xMove+1, yMove) != currentPlayer &&
        getColorAtLocation(xMove+1, yMove) != 'O') &&
        lookEast(xMove+1, yMove, currentPlayer, doFlip, flipTo)
        )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove+1, yMove-1) &&
        getColorAtLocation(xMove+1, yMove-1) != currentPlayer &&
        getColorAtLocation(xMove+1, yMove-1) != 'O') &&
        lookSE(xMove+1, yMove-1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove, yMove-1) &&
        getColorAtLocation(xMove, yMove-1) != currentPlayer &&
        getColorAtLocation(xMove, yMove-1) != 'O') &&
        lookSouth(xMove, yMove-1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove-1, yMove-1) &&
        getColorAtLocation(xMove-1, yMove-1) != currentPlayer &&
        getColorAtLocation(xMove-1, yMove-1) != 'O') &&
        lookSW(xMove-1, yMove-1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove-1, yMove) &&
        getColorAtLocation(xMove-1, yMove) != currentPlayer &&
        getColorAtLocation(xMove-1, yMove) != 'O') &&
        lookWest(xMove-1, yMove, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    if((inBounds(xMove-1, yMove+1) &&
        getColorAtLocation(xMove-1, yMove+1) != currentPlayer &&
        getColorAtLocation(xMove-1, yMove+1) != 'O') &&
        lookNW(xMove-1, yMove + 1, currentPlayer, doFlip, flipTo)
       )
    {
        checkIsValid = true;
    }

    return checkIsValid;
}

bool Board::lookNorth(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    #ifdef DEBUG
    cout << "DEBUG: Looking North. " << endl;
    #endif // DEBUG

    if(!inBounds(xMove,yMove))
    {
        #ifdef DEBUG
        cout << "DEBUG: North, not in bounds. " << endl;
        #endif // DEBUG
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if(colorHere == 'O')
    {
        #ifdef DEBUG
        cout << "DEBUG: North, Hit empty. " << endl;
        #endif // DEBUG
        return false;
    }

    else if(colorHere == currentPlayer)
    {
        #ifdef DEBUG
        cout << "DEBUG: North, Hit Same. Return True." << endl;
        #endif // DEBUG
         return true;
    }

    #ifdef DEBUG
    cout << "DEBUG: Creeping North..." << endl;
    #endif // DEBUG

//    setColorAtLocation('n', xMove, yMove);

    if (lookNorth(xMove, yMove+1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            #ifdef DEBUG
            cout << "DEBUG: Flipping "<< xMove<< " , " << yMove << "...";
            #endif // DEBUG

            setColorAtLocation(flipTo, xMove, yMove);

            #ifdef DEBUG
            cout << " Color flipped!"<< endl;
            #endif // DEBUG
        }

        return true;
    }

    return false;
}

bool Board::lookNE(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{

    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookNE(xMove+1, yMove+1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookEast(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    #ifdef DEBUG
    cout << "DEBUG: Looking East. " << endl;
    #endif // DEBUG

    if(!inBounds(xMove,yMove))
    {
        #ifdef DEBUG
        cout << "DEBUG: East Not in Bounds, " << endl;
        #endif // DEBUG
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    #ifdef DEBUG
    cout << "DEBUG: East, Color here is " << colorHere << " @ " << xMove << ", " << yMove<< endl;
    #endif // DEBUG

    if(colorHere == 'O')
    {
        #ifdef DEBUG
        cout << "DEBUG: East, Current location Is blank, " << endl;
        #endif // DEBUG

        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    #ifdef DEBUG
    cout << "DEBUG: Creeping East... " << endl;
    #endif // DEBUG

    if (lookEast(xMove+1, yMove, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            #ifdef DEBUG
            cout << "DEBUG: East Success, flipping... " << endl;
            #endif // DEBUG
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookSE(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookSE(xMove+1, yMove-1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookSouth(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookSouth(xMove, yMove-1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookSW(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookSW(xMove-1 , yMove-1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookWest(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookWest(xMove - 1, yMove, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }

        return true;
    }

    return false;
}

bool Board::lookNW(int xMove, int yMove, char currentPlayer, bool doFlip, char flipTo)
{
    if(!inBounds(xMove,yMove))
    {
        return false;
    }

    char colorHere = getColorAtLocation(xMove, yMove);

    if( colorHere == 'O')
    {
        return false;
    }

    else if(colorHere == currentPlayer)
    {
         return true;
    }

    else if (lookNW(xMove-1, yMove+1, currentPlayer, doFlip, flipTo))
    {
        if(doFlip)
        {
            setColorAtLocation(flipTo, xMove, yMove);
        }
        return true;
    }

    return false;
}

bool Board::inBounds(int xMove, int yMove)
{
    if(xMove > 7 or xMove < 0 or yMove > 7 or yMove < 0)
    {
        return false;
    }

    return true;
}

void Board::setBlackScore(int score)
{
    blackScore = score;
}

void Board::setWhiteScore(int score)
{
    whiteScore = score;
}


int Board::getBlackScore()
{
    return blackScore;
}

int Board::getWhiteScore()
{
    return whiteScore;
}

int Board::getSize()
{
	return mSize;
}

char Board::getOtherPlayer(char player)
{
	if(player == 'B')
	{
		return 'W';
	}
	
	return 'B';
}

