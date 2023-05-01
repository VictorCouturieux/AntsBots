#pragma once

#include "State.h"
#include "Route.h"

class FarmingLocation
{
public:
    State& state;
    
    std::vector<Location> farmingList;

    FarmingLocation(State& state);
    bool findClosestLoc(Location& loc);
};
