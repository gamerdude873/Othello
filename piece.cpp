// Authors: Aaron Marsh, Garret Sparks
// File: piece.cpp
// Last Updated: 3/29/13
// Description: member of board class piece implementation

/* ===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.

Aaron, 3/29: Added error padding (invalid piece color), added flip, isHealthy, DEBUG
====================================================================*/

#include <iostream>
#include "piece.h"

#define BOARDSIZE 7
// #define DEBUG

using namespace std;

Piece::Piece(char color, int xLocation, int yLocation) {
//    if (color != 'w' && color != 'b') {
//        throw color;
//    } else if(xLocation > 7 || xLocation < 0) {
//        throw xLocation;
//    } else if(yLocation > 7 || yLocation < 0) {
//        throw yLocation;
//    } else {
        mColor = color;
        mXLocation = xLocation;
        mYLocation = yLocation;
//    }
}

Piece::Piece() {
    mColor = 'O';
    mXLocation = 0;
    mYLocation = 0;
}

Piece::~Piece() {
}

char Piece::getColor() {
    return mColor;
}

bool Piece::setColor(char color)
 {
     // set the piece color, but make sure that it is a valid color first.
    // Returns true if successful, false otherwise.

    mColor = color;

    #ifdef DEBUG
    cout << "DEBUG: The color being set is: " << color << endl;
    #endif // DEBUG


    if(!isHealthy())
    {
        #ifdef DEBUG
        cout << "DEBUG: Bad choice of color. Setting to blank instead." << endl;
        #endif // DEBUG
        return false;
    }


    #ifdef DEBUG
    cout << "DEBUG: The color set was: " << getColor() << endl;
    #endif // DEBUG
    return true;
}

int Piece::getXLocation() {
    return mXLocation;
}

int Piece::getYLocation() {
    return mYLocation;
}

bool Piece::isHealthy()
{
    // health checkup for program integrity. We can't have a piece that isn't B, W, or O or N!
    if(mColor != 'W' and mColor != 'B' and mColor != 'O' and mColor != 'N' and mColor != 'n' and mColor != 'b' and mColor != 'w')
    {
        mColor = 'O';
        if(mColor != 'O')
        {
            cout << "Something's wrong with this piece! I can't fix it." << endl;
            return false;
        }
    }
    return true;
}


