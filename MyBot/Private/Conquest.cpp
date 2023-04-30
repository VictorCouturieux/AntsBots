#include "../Public/Conquest.h"
#include "../Public/Bot.h"

void Conquest::makeMoves()
{
    Behaviour::makeMoves();
    //bot->state.bug << "Conquest" << endl;

    
    /////       ***** Attacking ennemies *****      /////
    // attack hills
    vector<Route> hillRoutes;
    int ID=0;
    for (Location hillLoc : bot->state.enemyHills)
        for(Location ant : bot->state.myAnts)
            if (!bot->orders.containsValue(ant))
            {
                const double distance = bot->state.ManhattanDistance(ant, hillLoc);
                hillRoutes.emplace_back(ant, hillLoc, distance);
            }
    sort( hillRoutes.begin(), hillRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
    for (Route route : hillRoutes)
        doMoveLocation(route.Start, route.End);
    //bot->state.bug << "ant found road to hill" << endl;
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes( nbFood*nbAnts);
    ID=0;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.ManhattanDistance(ant, food);
            foodRoutes[ID++] = Route(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
        {
            doMoveLocation(food.Start, food.End);
            //bot->state.bug << "ant found road to hill" << endl;
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
                doMoveLocation(route.Start, route.End);
            //bot->state.bug << "ant found road to hill" << endl;
        }
    }

    // Moving out from our hills
    moveOutFromHills();
}
