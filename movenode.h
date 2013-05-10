//@file movenode.cpp
//@author Garrett Sparks (gsparks)

//@ Specification of ADT MoveNode
//@    data object is an item representing an x and y location on the board
//@    operations: create, destroy

#ifndef MOVENODE_H
#define MOVENODE_H
#include <cstdlib>
#include <string>
#include <list>
#include "board.h"

using namespace std;

class MoveNode

{
public:

    //@makes an empty item
    //@post item is empty
    //@usage MoveNode mvNd;
    MoveNode();

    MoveNode(char xLocation, char yLocation, int heuristicValue);

//    MoveNode(int xLocation, int yLocation, int, const Board &);

    //@destroys an item
    //@pre item exists
    //@post item does not exist
    //@usage automatically done at the end of scope
    ~MoveNode();

	char getXLocation();
	char getYLocation();
	void addMove(MoveNode *newMove);
	void addMove(char newXChar, char newYChar);
	int convertXCoord(char newX);
	int convertYCoord(char newY);
	char convertXInt(int);
	char convertYInt(int);
	void setPlayer(char);
	char getPlayer();
	int getHeuristicValue();
	MoveNode* getNextMove();
	MoveNode & operator=(const MoveNode & rhs);
	bool isTerminalNode();

protected:

	char mXLocation;
	char mYLocation;
	int mHeuristicValue;
	char mPlayer;
	MoveNode* nextMove;
};
#endif
