// Authors: Aaron Marsh, Garret Sparks
// File: board.h
// Last Updated: 3/29/13
// Description: Board for othello class header

/*===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.

Aaron, 3/30: changed mPieces list to a list of pieces.
====================================================================*/

#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <utility>
#include "piece.h"

class Board
{
public:
    Board();
	Board(int);
	~Board();
	Board& clone(Board& rhsBoard);
// 	char getColorAtLocation(char xCoordinate, int yCoordinate); Deprecated
	char getColorAtLocation(int xCoordinate, int yCoordinate);
	void setColorAtLocation(char, int, int);
	void placePiece(char color, char xCoordinate, char yCoordinate);
	bool updateBoard();
	void printBoard();
	int convertXCoord(char);
	int convertYCoord(char);
	bool blackHasMove();
	bool whiteHasMove();
	void setBlackHasMove(bool);
	void setWhiteHasMove(bool);
	bool lookAround(int, int, char, bool, char);
	bool lookNorth(int, int, char, bool, char);
	bool lookNE(int, int, char,  bool, char);
	bool lookEast(int, int, char,  bool, char);
	bool lookSE(int, int, char,  bool, char);
	bool lookSouth(int, int, char,  bool, char);
	bool lookSW(int, int, char,  bool, char);
	bool lookWest(int, int, char,  bool, char);
	bool lookNW(int, int, char,  bool, char);
	bool inBounds(int, int);
	void setBlackScore(int);
	void setWhiteScore(int);
	void calcScore();	
	int calcHeuristic(char, int, int);
	char getOtherPlayer(char);
	int getBlackScore();
	int getWhiteScore();
	int getSize();	

 //   Possible Feature:
//    void addMove(list, int, int);
//    void removeMove(list, int, int);
//    bool isEmpty(list);

private:
	Piece mPiecesList[8][8];
    int mSize;
    int blackScore;
    int whiteScore;
    bool blackMoveExists;
    bool whiteMoveExists;

 //   Possible Feature:
//    list <pair<int,int> > blackMoves;
//    list <pair<int,int> > whiteMoves;
};

#endif
