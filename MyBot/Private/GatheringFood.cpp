#include "../Public/GatheringFood.h"
#include "../Public/Bot.h"

void GatheringFood::makeMoves()
{
    Behaviour::makeMoves();
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.EuclideanDistance(ant, food);
            if(distance < 20)
                foodRoutes.emplace_back(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);

    /////       ***** Attacking ennemies *****      /////
    // attack hills
    vector<Route> hillRoutes;
    for (Location hillLoc : bot->state.enemyHills)
        for(Location ant : bot->state.myAnts)
            if (!bot->orders.containsValue(ant))
            {
                const double distance = bot->state.ManhattanDistance(ant, hillLoc);
                if(distance <= bot->state.viewradius * 2.5)
                    hillRoutes.emplace_back(ant, hillLoc, distance);
            }
    sort( hillRoutes.begin(), hillRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
    
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.Start, route.End, route.Distance <= bot->state.viewradius * 2.5);
    }
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    Exploration(bot->state.viewradius * 2.5f);
    // Moving out from our hills
    moveOutFromHills();

    // Ants without any orders : farmers ? 
    for(Location antLoc : bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!bot->orders.containsValue(antLoc))
        {
            bot->state.bug << antLoc.ToString() << " doesn't have any order..." << endl;
            //int randX = rand() % bot->state.rows;
            //int randY = rand() % bot->state.cols;
            //doMoveLocation(antLoc, Location(randX, randY), false);
        }
    }
}
