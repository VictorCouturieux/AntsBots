#pragma once

#include <array>
#include <algorithm>

#include "State.h"
#include "Route.h"
#include "TrackerMap.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;
    
    // Track what moves we have issued : First -> New Location, Second : Ant Location
    std::map< Location, Location > orders;
    // This map tracks the foods targets and associated ants
    TrackerMap<Location, Location> targets;
    // Track all location we have not seen during the game
    std::vector<Location> unseenLocations;
    // Track all enemy hills we have found
    std::vector<Location> enemyHillsFounded;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves
    
    bool doMoveDirection(const Location &antLoc, int dir); // Check if a location based on a direction is available, and make the move if it is
    bool doMoveLocation(const Location &antLoc, const Location &destLoc);
    
};
