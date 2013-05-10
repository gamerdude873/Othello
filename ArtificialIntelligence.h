#ifndef ARTIFICIALINTELLIGENCE_H
#define ARTIFICIALINTELLIGENCE_H

#include "movenode.h"
#include "board.h"

class ArtificialIntelligence
{
    public:
        ArtificialIntelligence();
        ~ArtificialIntelligence();
        void setPlayerColor(char newPlayerColor);
        char getPlayerColor();
        int convertXCoord(char);
        int convertYCoord(char);
        char convertXInt(int);
        char convertYInt(int);
        MoveNode chooseNextMove(Board &);
        char alternatePlayer(char);
        int buildMoveTree(MoveNode *, char, int, int, Board &, int, int, MoveNode *&);
		void panicDestroy(MoveNode * moveNode);

    protected:
        MoveNode* mRootNode;
        char playerColor;

    private:
};

#endif // ARTIFICIALINTELLIGENCE_H
