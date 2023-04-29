#include "../Public/GatheringFood.h"
#include "../Public/Bot.h"

void GatheringFood::makeMoves()
{
    Behaviour::makeMoves();
    //bot->state.bug << "Gather" << endl;
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes( nbFood*nbAnts);
    int ID=0;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.ManhattanDistance(ant, food);
            foodRoutes[ID++] = Route(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    //bot->state.bug << "Gather food" << endl;
    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            if (!doMoveLocation(food.Start, food.End, food.Distance < 20))
            {
                //bot->state.bug << "food to far" << endl;
            }

    /////       ***** Attacking ennemies *****      /////
    // add new hills to set
    for (Location enemyHill : bot->state.enemyHills)
        if (std::find(bot->enemyHillsFounded.begin(), bot->enemyHillsFounded.end(), enemyHill) == bot->enemyHillsFounded.end())
            bot->enemyHillsFounded.push_back(enemyHill);

    // attack hills
    vector<Route> hillRoutes;
    for (Location hillLoc : bot->enemyHillsFounded)
        for(Location ant : bot->state.myAnts)
            if (!bot->orders.containsValue(ant))
            {
                const double distance = bot->state.ManhattanDistance(ant, hillLoc);
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
            vector<Route> unseenRoutes;
            for (Location unseenLoc  : bot->unseenLocations)
            {
                const double distance = bot->state.ManhattanDistance(antLoc, unseenLoc);
                unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
            }
            
            sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

            for (Route route : unseenRoutes)
            {
                if(doMoveLocation(route.Start, route.End, unseenRoutes.size() < 10 && route.Distance <= bot->state.viewradius))
                {
                    //bot->state.bug << "ant found food" << endl;
                    break;
                }
            }
                
        }
        // else
        //     bot->state.bug << "Occupied..." << endl;
    }

    // Moving out from our hills
    moveOutFromHills();
}
