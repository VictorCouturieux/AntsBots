#include "../Public/GatheringFood.h"
#include "../Public/Bot.h"

void GatheringFood::MakeMoves()
{
    Behaviour::MakeMoves();
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes ;
    for(Location food : _bot->state.food)
        for(Location ant : _bot->state.myAnts)
        {
            const double distance = _bot->state.EuclideanDistance(ant, food);
            if(distance < 20)
                foodRoutes.emplace_back(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.distance < b.distance; } );

    //look if the food route is taken by a other ant
    for(Route food : foodRoutes)
        if(!_bot->targets.ContainsKey(food.end) && !_bot->targets.ContainsValue(food.start))
            doMoveLocation(food.start, food.end, true);

    /////       ***** Attacking ennemies hills *****      /////
    /// Attack hills
    vector<Route> hillRoutes = getShortestRoutesTo(_bot->state.enemyHills, _bot->state.viewRadius * 2);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.start, route.end, true);
    }
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    exploration(_bot->state.viewRadius * 2.5f);
    // Moving out from our hills
    moveOutFromHills();

    /////       ***** Random *****      /////
    // Ants without any orders : farmers ?
    for(Location antLoc : _bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!_bot->orders.ContainsValue(antLoc))
        {
            // search a random location where the ant can go
            int randX = rand() % _bot->state.rows;
            int randY = rand() % _bot->state.cols;
            //it the the random location is water it research a new random location 
            while(_bot->state.grid[randX][randY].isWater)
            {
                randX = rand() % _bot->state.rows;
                randY = rand() % _bot->state.cols;
            }
            doMoveLocation(antLoc, Location(randX, randY), false);
        }
    }
}

