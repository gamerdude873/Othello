// Authors: Aaron Marsh, Garret Sparks
// File: main.cpp
// Last Updated: 3/29/13
// Description: Program frontend intializer for Othello

/*===================================================================
Comments and Changelog:

Aaron, 3/29: Added comments/changelog etc.
====================================================================*/

#include <iostream>
#include <limits>
#include "othello.h"
#include "board.h"

// #define DEBUG

using namespace std;

int main() {
    Othello othelloGame;

    #ifdef DEBUG
    cout<< "DEBUG: Initializing board..." << endl;
    #endif // DEBUG

    othelloGame.playGame();

    // (Aaron) Adding this to make it easier to test on windows:

    cout <<  "press enter to continue..." << endl;
    cin.get();

    return 0;
}
