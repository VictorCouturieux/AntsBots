#include "../Public/Conquest.h"
#include "../Public/Bot.h"

void Conquest::makeMoves()
{
    Behaviour::makeMoves();
    //bot->state.bug << "Conquest" << endl;

    
    /////       ***** Attacking ennemies *****      /////
    // add new hills to set
    for (Location enemyHill : bot->state.enemyHills)
        if (std::find(bot->enemyHillsFounded.begin(), bot->enemyHillsFounded.end(), enemyHill) == bot->enemyHillsFounded.end())
            bot->enemyHillsFounded.push_back(enemyHill);

    // attack hills
    vector<Route> hillRoutes( nbAnts*bot->enemyHillsFounded.size());
    int ID=0;
    for (Location hillLoc : bot->enemyHillsFounded)
        for(Location ant : bot->state.myAnts)
            if (!bot->orders.containsValue(ant))
            {
                const double distance = bot->state.distance(ant, hillLoc);
                hillRoutes[ID++] = Route(ant, hillLoc, distance);
            }
    sort( hillRoutes.begin(), hillRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
    for (Route route : hillRoutes)
        doMoveLocation(route.Start, route.End);

    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes( nbFood*nbAnts);
    ID=0;
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
        }
    }

    // Moving out from our hills
    moveOutFromHills();
}
