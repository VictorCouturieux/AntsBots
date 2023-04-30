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
    for(Location antLoc : bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!bot->orders.containsValue(antLoc))
        {
            // If the ant was already previously exploring, we recover its destination and start the movement again
            if(bot->pathOrders.containsKey(antLoc))
            {
                Location destLoc = bot->pathOrders.GetMap()[antLoc][bot->pathOrders.GetMap()[antLoc].size()-1];
                doMoveLocation(antLoc, destLoc, true);
            }
            // Otherwise, we'll want to build a new exploring path
            else
            {
                vector<Route> unseenRoutes;
                for (Location unseenLoc  : bot->unseenLocations)
                {
                    // If the targeted unseen location is water, we remove it from the vector, as destination is unreachable
                    if(bot->state.grid[unseenLoc.row][unseenLoc.col].isWater)
                        remove(bot->unseenLocations.begin(), bot->unseenLocations.end(), unseenLoc);

                    // Register the location as a route if the destination is not too far
                    else
                    {
                        const double distance = bot->state.ManhattanDistance(antLoc, unseenLoc);
                        if(distance<= bot->state.viewradius * 2.5f)
                            unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
                    }
                }

                // Sort to have the lower distances first
                sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

                for (Route route : unseenRoutes)
                {
                    if(doMoveLocation(route.Start, route.End, route.Distance <= bot->state.viewradius * 2.5f))
                    {
                        break;
                    }
                }
            }  
        }
    }
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
