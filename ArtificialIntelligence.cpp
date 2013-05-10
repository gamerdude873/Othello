#include "ArtificialIntelligence.h"
#include "board.h"
#include "movenode.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <ctime>
#include <csignal>

//#define DEBUG
#define TIME
//#define SCOREDEBUG
//#define SCOREDEBUG2
//#define DECISIONDEBUG
// #define MEMDEBUG

bool panicFlag;

ArtificialIntelligence::ArtificialIntelligence()
{
    mRootNode = new MoveNode;
    panicFlag = false;
}

ArtificialIntelligence::~ArtificialIntelligence()
{
    //dtor
}

void catchAlarm(int sig)
{
    cout << "AI: PANICKING, TAKING TOO LONG!" << endl;
    panicFlag = true;
}

MoveNode ArtificialIntelligence::chooseNextMove(Board & gameboard)
{
    #ifdef TIME
    time_t start;
    time_t  end;
    #endif // TIME

    int INFINITY = std::numeric_limits<int>::max();
    int NEG_INFINITY = std::numeric_limits<int>::min();

	MoveNode nextMove; 
	MoveNode * decision = NULL;

	//maximum depth to search ahead
    int maxDepth = 8;

    #ifdef TIME
    time(&start);
    #endif // TIME

    //create an alarm signal to process the SIGALRM return
    signal(SIGALRM, catchAlarm);
    alarm(85);  //start an alarm that lasts n seconds

    //builds a pruned tree
	buildMoveTree(mRootNode, playerColor, maxDepth, 0, gameboard, NEG_INFINITY, INFINITY, decision);

  	alarm(0);  //cancel the alarm after the buildMoveTree is complete
	panicFlag = false;

	if(decision == NULL)
	{
		cout << "AI: Decision not Made! I'm screwed!" << endl;
	}

    nextMove = *decision;

	#ifdef SCOREDEBUG2
	cout << "DEBUG: the score of the decision is " << nextMove.getHeuristicValue() << endl;
	#endif


	#ifdef MEMDEBUG
	cout << "MEMDEBUG: Root cleanup..." <<endl;	
	#endif

	delete mRootNode;
	mRootNode = new MoveNode;

	#ifdef MEMDEBUG
	cout << "MEMDEBUG: Root reset." <<endl;	
	#endif


//    panicFlag = false;  //make sure panicFlag is false again, just in case the alarm was triggered

    #ifdef TIME
    time(&end);

    cout << "TIME: That decision took " << difftime(end,start) << " seconds to make." << endl;
    #endif // TIME

	return nextMove;
}

void ArtificialIntelligence::setPlayerColor(char newPlayerColor)
{
    playerColor = newPlayerColor;
}

char ArtificialIntelligence::getPlayerColor()
{
    return playerColor;
}

int ArtificialIntelligence::convertXCoord(char xCoord)
{
     // convert ascii to int array index Y coordinate is already int, but we need to convert to array indexing
    xCoord = tolower(xCoord);                                            // need lowercase
    return xCoord - 97;
}

int ArtificialIntelligence::convertYCoord(char yCoord)
{
    return yCoord - 49;
}

void ArtificialIntelligence::panicDestroy(MoveNode * moveNode)
{
	if(moveNode -> getNextMove() == NULL && moveNode != mRootNode)
	{
		delete moveNode;
		moveNode = NULL;
	}
	else if(moveNode != mRootNode)
	{
		panicDestroy(moveNode -> getNextMove());
		delete moveNode;
		moveNode = NULL;
	}
}

int ArtificialIntelligence::buildMoveTree(MoveNode * lastMove, char player, int maxDepth, int atDepth, Board & currentBoard, int alpha, int beta, MoveNode *& decision)
{
    int maxAlpha = std::numeric_limits<int>::min();
	Board copyBoard(7);
	MoveNode * move;
	MoveNode * ground = NULL;
	bool moveFound = false;

	#ifdef DEBUG
	cout << "DEBUG: current player is " << player << " at height " << growToDepth << endl;
	#endif

    //if the alarm was triggered, setting panicFlag to true, make the decision root's first child

    if(panicFlag) {
		MoveNode panicDecision = *mRootNode -> getNextMove();
		panicDestroy(mRootNode);
		decision = &panicDecision;
        return 0;
    }

    //otherwise, continue through the recursion like usual
        atDepth++; // where am i?

        if((atDepth == maxDepth) && (lastMove != mRootNode))
        {
            #ifdef SCOREDEBUG
            cout <<  "DEBUG: H-Val is " << lastMove -> getHeuristicValue() << " at Bottom."<< endl;
            #endif // SCOREDEBUG

            return lastMove -> getHeuristicValue();
        }


        for(int i = 0; i <= 7; i++)
        {
            for(int j = 0; j <= 7; j++)
            {
                if(currentBoard.lookAround(i,j, player, false, player))
                {
                    // generate a move
                    copyBoard.clone(currentBoard);
                    copyBoard.lookAround(i,j, player, true, player);
                    copyBoard.placePiece(player, convertXInt(i), convertYInt(j));
                    copyBoard.calcScore();
                    move = new MoveNode(convertXInt(i), convertYInt(j), copyBoard.calcHeuristic(playerColor, i, j));
                    move -> setPlayer(player);
                    lastMove -> addMove(move);
                    moveFound = true;
					
                    #ifdef DEBUG

                    cout << "DEBUG: A move for " << player << " at " << convertXInt(i) << "," << convertYInt(j) << " at height " << growToDepth + 1 << endl;
                    #endif

                    // leaf Node, don't make children

                    // Not a leaf,

                    if(player != playerColor)
                    {
                            #ifdef DECISIONDEBUG
                            cout << "DEBUG: Looking for  beta" << endl;
                            #endif // DECISIONDEBUG

                            beta = min(beta, buildMoveTree(move, alternatePlayer(player), maxDepth, atDepth, copyBoard, alpha, beta, ground)) + move-> getHeuristicValue()/(atDepth*atDepth);

                            #ifdef DECISIONDEBUG
                            cout << "DEBUG: beta is " << beta << endl;
                            #endif // DECISIONDEBUG
                    }

                    else
                    {

                            #ifdef DECISIONDEBUG
                            cout << "DEBUG: Looking for  alpha" << endl;
                            #endif // DECISIONDEBUG

                            alpha = max(alpha, buildMoveTree(move, alternatePlayer(player), maxDepth, atDepth, copyBoard, alpha, beta, ground))+ move -> getHeuristicValue()/(atDepth*atDepth);

                            #ifdef DECISIONDEBUG
                            cout << "DEBUG: alpha is " << alpha << endl;
                            #endif // DECISIONDEBUG

                            #ifdef DECISIONDEBUG
                            if(atDepth == 1)
                            {
                                 cout << "DEBUG: Considering a decision... Score is " << alpha  << " at position " << move -> getXLocation() << ", " << move->getYLocation() << endl;
                            }
                            #endif // DECISIONDEBUG

                            if((atDepth == 1) && (alpha >= maxAlpha))
                            {
                                        // Where do I go? The biggest alpha.
                                        maxAlpha = alpha;
										
										if(decision != NULL)
										{
											#ifdef DECISIONDEBUG
											cout << "DEBUG: Attempting to delete old decision..." <<endl;	
											#endif
											delete decision;
										}

										#ifdef DECISIONDEBUG
										cout << "DEBUG: Old decision forgotten. Assigning new decision..." <<endl;	
										#endif
										
                                        mRootNode -> addMove(move);
										decision = mRootNode -> getNextMove();

										#ifdef DECISIONDEBUG
										cout << "DEBUG: New decision in hand. I'm now going to " << decision -> getXLocation() << ", " << decision->getYLocation() << " which has a score of " << alpha << endl;	
										#endif
                            }

                    }

					if((move != decision) && (move != mRootNode))// keep decisions and the root, destroy everything else as we come back up.


					{
						#ifdef MEMDEBUG
						cout << "MEMDEBUG: deleting child ..." <<endl;	
						#endif

						delete move;
						move = NULL;

						#ifdef MEMDEBUG
						cout << "MEMDEBUG: Child forgotten." <<endl;	
						#endif
					}

                    // keep immediate children of the root to make a move, but otherwise destroy all the nodes below after we are done with them.

                    // if alpha >= beta, stop making children (Breaks loop)
                    if((alpha >= beta) && (player == playerColor))
                    {
                        return alpha;
                    }

                    else if((alpha >= beta) && (player != playerColor))
                    {
                        return beta;
                    }
                }
            } // End j loop
   		 } // end i loop


		// no cuts case.
        if((player == playerColor) && moveFound)
        {
            return alpha;
        }

        else if((player != playerColor) && moveFound)
        {
            return beta;
        }

  		// case where we don't have children but are not at the max depth bound.
        return lastMove -> getHeuristicValue();	
}

char ArtificialIntelligence::convertXInt(int xCoord)
{
    return toupper(xCoord + 97);
}

char ArtificialIntelligence::convertYInt(int yCoord)
{
    return yCoord + 49;
}

char ArtificialIntelligence::alternatePlayer(char player)
{
	if(player == 'B')
	{
		#ifdef DEBUG
		cout<< "DEBUG:flopping player from " << player << " to ";
		#endif

		return 'W';

		#ifdef DEBUG
		cout<<  player << endl;
		#endif

	}

	else
	{
		#ifdef DEBUG
		cout<< "DEBUG:flopping player from " << player << " to ";
		#endif

		return 'B';

		#ifdef DEBUG
		cout<<  player << endl;
		#endif
	}
}
