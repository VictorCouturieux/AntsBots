#pragma once

#include <map>

#include "State.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;
    
    // Track what moves we have issued
    std::map< Location, Location > orders;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves
    
    bool doMoveDirection(const Location &antLoc, int dir); // Check if a location based on a direction is available, and make the move if it is
    bool doMoveLocation(const Location &antLoc, const Location &destLoc);
    
};
