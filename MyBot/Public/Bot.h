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
    // Track all enemy hills we have found
    std::vector<Location> enemyHillsFounded;

    std::map<GameState, Behaviour*> Behaviours;

    Bot();

    void playGame();    //plays a single game of Ants
    void checkAntPath();

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves    
};
