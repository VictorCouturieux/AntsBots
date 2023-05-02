#include "../Public/GatheringFood.h"
#include "../Public/Bot.h"

void GatheringFood::makeMoves()
{
    Behaviour::makeMoves();
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes ;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.EuclideanDistance(ant, food);
            if(distance < 20)
                foodRoutes.emplace_back(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    //look if the food route is taken by a other ant
    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);

    /////       ***** Attacking ennemies hills *****      /////
    /// Attack hills
    vector<Route> hillRoutes = getShortestRoutesTo(bot->state.enemyHills, bot->state.viewradius * 2);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    Exploration(bot->state.viewradius * 2.5f);
    // Moving out from our hills
    moveOutFromHills();

    /////       ***** Random *****      /////
    // Ants without any orders : farmers ?
    for(Location antLoc : bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!bot->orders.containsValue(antLoc))
        {
            // search a random location where the ant can go
            int randX = rand() % bot->state.rows;
            int randY = rand() % bot->state.cols;
            //it the the random location is water it research a new random location 
            while(bot->state.grid[randX][randY].isWater)
            {
                randX = rand() % bot->state.rows;
                randY = rand() % bot->state.cols;
            }
            doMoveLocation(antLoc, Location(randX, randY), false);
        }
    }
}

