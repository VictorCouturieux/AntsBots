#include "../Public/DefendHomeland.h"
#include "../Public/Bot.h"

void DefendHomeland::makeMoves()
{
    Behaviour::makeMoves();
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes( nbFood*nbAnts);
    int ID=0;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.distance(ant, food);
            foodRoutes[ID++] = Route(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End);
    //bot->state.bug << "ant DefHomeLand" << endl;

    
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
                const double distance = bot->state.distance(antLoc, unseenLoc);
                unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
            }
            sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
            for (Route route : unseenRoutes)
                doMoveLocation(route.Start, route.End);
            //bot->state.bug << "ant DefHomeLand" << endl;
        }
    }

    // Moving out from our hills
    moveOutFromHills();
}
