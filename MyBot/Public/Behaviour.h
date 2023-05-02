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

    /**
     * \brief Make the move using according to the chosen behaviour 
     */
    virtual void makeMoves();
    virtual void Init() {}

    GameState type;
    int nbFood;
    int nbAnts;
    
protected :
    Bot* bot;
    AStarAlgo* aStarPathFinding;

    /**
     * \brief Takes a Ant location and a direction and tries to perform the move
     * \param antLoc Ant location
     * \param dir Direction
     * \return True if the ant has moved
     */
    bool doMoveDirection(Location antLoc, int dir) const;

    /**
     * \brief Tries to move an Ant towards a destination
     * \param antLoc Ant location
     * \param destLoc Destination location
     * \param pathFinding if the pathfinding is required
     * \return True if the ant has moved
     */
    bool doMoveLocation(Location antLoc, Location destLoc, bool pathFinding=true) const;
    
    /**
     * \brief Retrieve the routes of all available ants to all destinations, sorted by the lowest euclidean distances
     * \param destinations Destinations to check 
     * \param range Allowed range around an ant position
     * \return Sorted routes
     */
    vector<Route> getShortestRoutesTo(vector<Location> destinations, double range) const;

    /**
     * \brief Set some ant to explore the map
     * \param range Allowed range around an ant position
     */
    void Exploration(float range) const;
 
    /**
     * \brief Move ants that are still on a hill
     */
    void moveOutFromHills() const;
};
