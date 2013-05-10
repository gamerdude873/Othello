// Authors: Aaron Marsh, Garret Sparks
// File: othello.h
// Last Updated: 3/29/13
// Description: othello game class header

/* ===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.
====================================================================*/

#ifndef OTHELLO_H
#define OTHELLO_H
#include "board.h"
#include "movenode.h"
#include "ArtificialIntelligence.h"

class Othello
{
public:
	Othello();
	~Othello();
	void playGame();
	void initializeBoard();
	void gameLoop(char, ArtificialIntelligence &aiPlayer);
	void makeAIMove(ArtificialIntelligence& aiPlayer, char &, bool &);
	void playerTurn(char &, bool &);
	void endOfTurn(char &, bool &);

private:
    Board gameboard;

};
#endif

