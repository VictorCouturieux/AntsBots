#pragma once

#include <array>
#include <algorithm>

#include "State.h"
#include "Route.h"
#include "GatheringFood.h"
#include "DefendHomeland.h"
#include "Conquest.h"
#include "TrackerMap.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;
    GameState gameState = Food;
    
    // Track what moves we have issued : First -> New Location, Second : Ant Location
    TrackerMap<Location, Location> orders;

    // Order list by ants calculate by A* algo : optimise A* aglo utility 
    TrackerMap<Location, std::vector<Location>> pathOrders;
    
    // This map tracks the foods targets and associated ants
    TrackerMap<Location, Location> targets;
    
    // Track all location we have not seen during the game
    std::vector<Location> unseenLocations;

    // Behaviours list execute in order
    std::map<GameState, Behaviour*> Behaviours;

    Bot(){}
    
    /**
     * \brief plays a single game of Ants.
     */
    void playGame(); 

    /**
     * \brief makes moves for a single turn
     */
    void makeMoves(); 

    /**
     * \brief indicates to the engine that it has made its moves    
     */
    void endTurn();

    /**
     * \brief check in the pathOrders list if an path have no ant reference
     * in case, it erase this one
     * it is used to remove a pathOrders if an ant will be dead
     */
    void checkAntPath();
};
