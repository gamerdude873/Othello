//@file movenode.cpp
//@author Garrett Sparks (gsparks)

//@ Implementation of ADT MoveNode
//@    data object is an item representing an x and y location on the board
//@    data structures: integers for the location and a list of pointers to subsequent moves
//@    operations: create, destroy

#include "movenode.h"
#include "board.h"
#include <cstdlib>
#include <string>
#include <cctype> //to upper

//@makes an empty item
//@post item is empty
//@usage MoveNode mvNd;
MoveNode::MoveNode()
{
	mXLocation = 'R';
	mYLocation = '0';
	mHeuristicValue = -1;
   	nextMove = NULL;
}

//@destroys an item
//@pre item exists
//@post item does not exist
//@usage automatically done at the end of scope
MoveNode::~MoveNode()
{
	nextMove = NULL;
}

MoveNode::MoveNode(char xLocation, char yLocation, int heuristicValue)
{
	mXLocation = xLocation;
	mYLocation = yLocation;
	mHeuristicValue = heuristicValue;
	nextMove = NULL;
}

//@returns the xLocation
//@pre item exists
//@post mXLocation is returned
//@usage someItem.getXLocation();
char MoveNode::getXLocation()
{
   return mXLocation;
}

//@returns the xLocation
//@pre item exists
//@post mXLocation is returned
//@usage someItem.getXLocation();s
char MoveNode::getYLocation()
{
    return mYLocation;
}

void MoveNode::addMove(MoveNode * newMove)
{
    nextMove = newMove;
}

int MoveNode::convertXCoord(char xCoord)
{
     // convert ascii to int array index Y coordinate is already int, but we need to convert to array indexing
    xCoord = tolower(xCoord);                                            // need lowercase
    return xCoord - 97;
}

int MoveNode::convertYCoord(char yCoord)
{
    return yCoord - 49;
}

char MoveNode::convertXInt(int xCoord)
{
    return toupper(xCoord + 97);
}

char MoveNode::convertYInt(int yCoord)
{
    return yCoord + 49;
}

char MoveNode::getPlayer()
{
	return mPlayer;
}

void MoveNode::setPlayer(char player)
{
	mPlayer = player;
}

int MoveNode::getHeuristicValue()
{
	return mHeuristicValue;
}

MoveNode* MoveNode::getNextMove()
{
	return nextMove;
}	

bool MoveNode::isTerminalNode()
{
	if(nextMove == NULL)
	{
		return true;
	}	
	
    return false;
}

MoveNode & MoveNode::operator=(const MoveNode & rhs)
{
	mXLocation = rhs.mXLocation;
	mYLocation = rhs.mYLocation;
	mHeuristicValue = rhs.mHeuristicValue;
	mPlayer = rhs.mPlayer;
	nextMove = rhs.nextMove;

	return *this;
}
