#pragma once

#include <algorithm>

#include "AStarAlgo.h"
#include "State.h"
#include "Route.h"
#include "TrackerMap.h"

struct Bot;
using namespace std;

class Behaviour
{
public:
    Behaviour(Bot* _bot, GameState _type);
    virtual void makeMoves();

    GameState type;
    int nbFood;
    int nbAnts;
    
protected :
    Bot* bot;
    AStarAlgo* aStarPathFinding;
    
    bool doMoveDirection(const Location &antLoc, int dir); // Check if a location based on a direction is available, and make the move if it is
    bool doMoveLocation(const Location &antLoc, const Location &destLoc);

    // Moving out from our hills
    void moveOutFromHills();
};
