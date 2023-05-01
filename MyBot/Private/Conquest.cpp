#include "../Public/Conquest.h"
#include "../Public/Bot.h"

void Conquest::Init()
{
    Behaviour::Init();
    
    if(!Initialised && !bot->state.myHills.empty())
    {
        int range = 2;
        
        for (Location hillLoc : bot->state.myHills)
        {
            for(int x = hillLoc.row - range; x < hillLoc.row + range; x++)
            {
                if(x > bot->state.rows) break; // Out of map limits
                for(int y = hillLoc.col - range; y < hillLoc.col + range; y++)
                {
                    if(x == hillLoc.row && y == hillLoc.col) break; // hill loc
                    if(y > bot->state.cols) break; // Out of map limits
                    if(bot->state.grid[x][y].isWater) break;
                    if(bot->state.ManhattanDistance(hillLoc, Location(x,y)) <= range)
                        Protectors.emplace_back(x,y);
                }
            }
        }
        Initialised = true;
    }
    
}

void Conquest::makeMoves()
{
    Behaviour::makeMoves();
    //bot->state.bug << "Conquest" << endl;

    /////       ***** Attacking ennemies *****      /////
    /// Assign some ant to protect hills
    /*for (Location hillLoc : bot->state.myHills)
    {
        vector<Route> closeAnts;
        for(Location ant : bot->state.myAnts)
            if (!bot->orders.containsValue(ant))
            {
                const double distance = bot->state.ManhattanDistance(ant, hillLoc);
                if(distance <= bot->state.viewradius)
                    closeAnts.emplace_back(ant, hillLoc, distance);
            }
        sort( closeAnts.begin(), closeAnts.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
        int id=0;
        for(Route route : closeAnts)
        {
            if(id > Protectors.size()) break;
            doMoveLocation(route.Start, Protectors[id++], false);
        }
    }*/
    /// Attack ants
    vector<Route> enemyRoutes = getShortestRoutesTo(bot->state.enemyAnts, bot->state.viewradius);
    for (Route route : enemyRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }

    /// Attack hills
    vector<Route> hillRoutes = getShortestRoutesTo(bot->state.enemyHills, bot->state.viewradius * 2.5);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }
    
    /////       ***** Food gathering ***** 
    vector<Route> foodRoutes = getShortestRoutesTo(bot->state.food, 20);
    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    Exploration(bot->state.viewradius / 2.0f);

    // Moving out from our hills
    moveOutFromHills();
}
