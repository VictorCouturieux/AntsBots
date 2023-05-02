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
                    if(x == hillLoc.row && y == hillLoc.col) break; // Hill loc
                    if(y > bot->state.cols) break; // Out of map limits
                    if(bot->state.grid[x][y].isWater) break; // Collision 
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
    /// TODO : Assign some ant to protect our hills
    
    /// Attack ants if they're close enough
    vector<Route> enemyRoutes = getShortestRoutesTo(bot->state.enemyAnts, bot->state.viewradius);
    for (Route route : enemyRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }

    /// Attack hills in a long range
    vector<Route> hillRoutes = getShortestRoutesTo(bot->state.enemyHills, bot->state.viewradius * 2.5);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }
    
    /////       ***** Food gathering ***** 
    // Send ants to gather food if they're not busy yet
    vector<Route> foodRoutes = getShortestRoutesTo(bot->state.food, 20);
    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);
    
    /////       ***** Exploration *****      /////
    // Explore close unseen areas
    Exploration(bot->state.viewradius / 2.0f);

    // Moving out from our hills
    moveOutFromHills();
}
