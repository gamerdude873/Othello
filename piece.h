// Authors: Aaron Marsh, Garret Sparks
// File: piece.cpp
// Last Updated: 3/29/13
// Description: member of board class piece header

/* ===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.

Aaron, 3/30: Added isHealthy() and changed setPiece to boolean. Added flip()
====================================================================*/

#ifndef PIECE_H
#define PIECE_H

using namespace std;

class Piece
{
public:
	Piece(char color, int xLocation, int yLocation);
	Piece();
	~Piece();
	char getColor();
	int getXLocation();
	int getYLocation();
	bool setColor(char color);
	bool isHealthy();

private:
	char mColor;
	int mXLocation;
	int mYLocation;
};
#endif
