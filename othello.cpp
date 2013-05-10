// Authors: Aaron Marsh, Garret Sparks
// File: othello.cpp
// Last Updated: 3/29/13
// Description: othello game class implementation

/*===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.

Aaron, 3/31: Greatly expanded game loop and piece placement checks, quashed bugs, tried to account for the million and one ways a player can break the game with bad I/O.

Aaron, 4/24: Quashed a serious bug where chips were flipping when they shouldn't after wanting to redo a move.

====================================================================*/

#include <iostream>
#include <cstdlib> // exit()
#include <utility>
#include "othello.h"
#include "board.h"
#include "movenode.h"
#include "ArtificialIntelligence.h"

// #define DEBUG

using namespace std;

Othello::Othello()
{
    gameboard = Board(7);
}

Othello::~Othello() {
}

void Othello::playGame()
{
	char currentPlayer;
	char ourPlayer;
	bool validInput = false;
	ArtificialIntelligence aiPlayer;

	initializeBoard();

	while (!validInput)
	{
		gameboard.updateBoard();
		gameboard.printBoard();
		cout << "System: Welcome to Othello! To stop the game at any time, enter 'S'. " << endl;
		cout << "Other Player, would you like to be (B)lack or (W)hite?" << endl;
		cin >> currentPlayer;
		currentPlayer = toupper(currentPlayer);

		if(currentPlayer == 's' or currentPlayer == 'S')
		{
		    cout << "System: Goodbye!" << endl;
		    exit(EXIT_SUCCESS);
		}

		else if (currentPlayer == 'B') {
		    cout << "Other Player chooses Black, let them move first." << endl;
		    currentPlayer = 'B';
		    ourPlayer = 'W';
		    aiPlayer.setPlayerColor(ourPlayer);
		    validInput = true;
		}

		else if (currentPlayer == 'W')
		{
		    char confirm;
		    cout << "Other Player chooses White, we're going first." << endl;
		    currentPlayer = 'B';
		    ourPlayer = 'B';
		    aiPlayer.setPlayerColor(ourPlayer);
		    cout << "Ok for us to move?" << endl;
		    cin >> confirm;
		    confirm = toupper(confirm);
		    if(confirm == 'Y')
		    {
			validInput = true;
		    }
		    else
		    {
			validInput = false;
		    }
		}

		else
		{
		    validInput = false;
		}
	}

	gameLoop(currentPlayer, aiPlayer);

	// game is over
	cout << "System: Goodbye!" << endl;
}

void Othello::initializeBoard()
{
	gameboard.placePiece('B', 'E', '5');
	gameboard.placePiece('W', 'E', '4');
	gameboard.placePiece('W', 'D', '5');
	gameboard.placePiece('B', 'D', '4');
	gameboard.setBlackScore(2);
	gameboard.setWhiteScore(2);
}

void Othello::gameLoop(char currentPlayer, ArtificialIntelligence & aiPlayer)
{
	bool running = true;

	while(running) //Gameloop begin
	{

		//structure allows for forfeiting of turn based on what endgame sets the current player to

		if((currentPlayer == aiPlayer.getPlayerColor()) && running)
		{
			if(currentPlayer == 'B')
			{
				cout << "System: Black Player's Turn!" << endl;
			}

			else
			{
				cout << "System: White Player's Turn!" << endl;
			}

			// AI/HOME MOVE
			makeAIMove(aiPlayer, currentPlayer, running);
		}

		if((currentPlayer != aiPlayer.getPlayerColor()) && running)
		{
			if(currentPlayer == 'B')
			{
				cout << "System: Black Player's Turn!" << endl;
			}

			else
			{
				cout << "System: White Player's Turn!" << endl;
			}

			// PLAYER/GUEST MOVE
			playerTurn(currentPlayer, running);
		}

	} // end of gameloop
}


void Othello::makeAIMove(ArtificialIntelligence& aiPlayer, char & currentPlayer, bool & running)
{
    cout << "AI: Puny opponent is instructed to wait while I look into the future." << endl;
	MoveNode nextMove = aiPlayer.chooseNextMove(gameboard);

	gameboard.lookAround(nextMove.convertXCoord(nextMove.getXLocation()), nextMove.convertYCoord(nextMove.getYLocation()), currentPlayer, true, currentPlayer);
	gameboard.placePiece(currentPlayer, nextMove.getXLocation(), nextMove.getYLocation());

	#ifdef DEBUG
	gameboard.printBoard();
	cout << "DEBUG: AI PLAYER JUST WENT" << endl;
	#endif

	cout << "System: AI went to " << nextMove.getXLocation() << ", " << nextMove.getYLocation() << endl;

	endOfTurn(currentPlayer, running);
}

void Othello::playerTurn(char & currentPlayer, bool & running)
{
	char yMove;
	char xMove;
	char ans;
	bool goodPlacement = false;
	int xIntMove;
	int yIntMove;
	Board highlightBoard = Board(7);
	highlightBoard.clone(gameboard);

	do{
	    cout <<"System: Write the letter of the (x-axis) position you want to play on!" << endl;
	    cin >> xMove;

	    if(xMove == 's' or xMove == 'S')
	    {
		cout << "System: Goodbye!" << endl;
		exit(EXIT_SUCCESS);
	    }

	    cout << "System: Write the number of the (y-axis) position you want to play on!" << endl;
	    cin >> yMove;

	    if(yMove == 's' or yMove == 'S')
	    {
		cout << "System: Goodbye!" << endl;
		exit(EXIT_SUCCESS);
	    }

	    xIntMove = gameboard.convertXCoord(xMove);
	    yIntMove = gameboard.convertYCoord(yMove);

	    if(gameboard.inBounds(xIntMove, yIntMove) && gameboard.getColorAtLocation(xIntMove, yIntMove) == 'O' )
	    {
		highlightBoard.clone(gameboard);

		if(currentPlayer == 'W')
		{
		    goodPlacement = highlightBoard.lookAround(xIntMove, yIntMove, currentPlayer, true, 'b');
		}

		else if (currentPlayer == 'B')
		{
		    goodPlacement = highlightBoard.lookAround(xIntMove, yIntMove, currentPlayer, true, 'w');
		}

		highlightBoard.placePiece(tolower(currentPlayer), xMove, yMove);
		highlightBoard.printBoard();
		cout << "System: You want to put a piece at " << xMove << ", " << yMove << ", now shown on this board; Is this correct? (Y/N)" << endl;
		cin >> ans;


		if(ans == 'S' || ans == 's')
		{
		    cout << "System: Goodbye!" << endl;
		    exit(EXIT_SUCCESS);
		}

		else if (!(ans == 'Y' || ans == 'y'))
		{
		    gameboard.placePiece('O', xMove, yMove);
		    cout << "System: Then Choose another move." << endl;
		    gameboard.printBoard();
		}

		else
		{
		    goodPlacement = gameboard.lookAround(xIntMove, yIntMove, currentPlayer, true, currentPlayer);
		}

		if(goodPlacement)
		{
		    gameboard.placePiece('O', xMove, yMove);
		}

		else if (!goodPlacement)
		{
		    gameboard.placePiece('O', xMove, yMove);
		    gameboard.printBoard();
		    cout << "System: The move is invalid. Try again!" << endl;
		}
	    }

	    else
	    {
		    cout << "System: Bad piece placement. Try again." << endl;
		    ans = 'N';
		    goodPlacement = false;
	    }

	} while(!goodPlacement || !(ans == 'Y' || ans == 'y'));

	ans = 'Y';

	cout << "System: Placing a "<< currentPlayer <<" piece at " << xMove << ", " << yMove << " and flipping pieces..."<< endl;
	gameboard.placePiece(currentPlayer, xMove, yMove);

	endOfTurn(currentPlayer, running);

}

void Othello::endOfTurn(char & currentPlayer, bool & running)
{
		char ans;
	//	bool doCheck = false; 
        running = gameboard.updateBoard();

        //Normal turn tradeoff
        // black just went and white has a move to make.
        if (currentPlayer == 'B' && gameboard.whiteHasMove())
        {
            currentPlayer = 'W';
//			doCheck = true;
        }

        //opposite
        else if (currentPlayer == 'W' && gameboard.blackHasMove())
        {
            currentPlayer = 'B';
//			doCheck = true;
        }

        gameboard.printBoard();

        // What if white doesn't have a move, but black just went?
        if (currentPlayer == 'B' && !gameboard.whiteHasMove() && gameboard.blackHasMove())
        {
            cout << "System: the white player has no moves to make. Therefore, white forfeits the turn." << endl;
            cout << "System: Okay to proceed?" << endl;
            cin >> ans;
        }

        // What if black doesn't have a move, but white just went?
        else if (currentPlayer == 'W' && gameboard.whiteHasMove() && !gameboard.blackHasMove())
        {
            cout << "System: the black player has no moves to make. Therefore, black forfeits the turn." << endl;
            cout << "System: Okay to proceed?" << endl;
            cin >> ans;
        }
}
