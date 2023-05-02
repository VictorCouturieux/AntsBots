#include "../Public/Conquest.h"
#include "../Public/Bot.h"

void Conquest::Init()
{
    Behaviour::Init();
    
    if(!_initialised && !_bot->state.myHills.empty())
    {
        int range = 2;
        
        for (Location hillLoc : _bot->state.myHills)
        {
            for(int x = hillLoc.row - range; x < hillLoc.row + range; x++)
            {
                if(x > _bot->state.rows) break; // Out of map limits
                for(int y = hillLoc.col - range; y < hillLoc.col + range; y++)
                {
                    if(x == hillLoc.row && y == hillLoc.col) break; // Hill loc
                    if(y > _bot->state.cols) break; // Out of map limits
                    if(_bot->state.grid[x][y].isWater) break; // Collision 
                    if(_bot->state.ManhattanDistance(hillLoc, Location(x,y)) <= range)
                        _protectors.emplace_back(x,y);
                }
            }
        }
        _initialised = true;
    }
    
}

void Conquest::MakeMoves()
{
    Behaviour::MakeMoves();
    //bot->state.bug << "Conquest" << endl;

    /////       ***** Attacking ennemies *****      /////
    /// TODO : Assign some ant to protect our hills
    
    /// Attack ants if they're close enough
    vector<Route> enemyRoutes = getShortestRoutesTo(_bot->state.enemyAnts, _bot->state.viewRadius);
    for (Route route : enemyRoutes)
    {
        doMoveLocation(route.start, route.end, true);
    }

    /// Attack hills in a long range
    vector<Route> hillRoutes = getShortestRoutesTo(_bot->state.enemyHills, _bot->state.viewRadius * 2.5);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.start, route.end, true);
    }
    
    /////       ***** Food gathering ***** 
    // Send ants to gather food if they're not busy yet
    vector<Route> foodRoutes = getShortestRoutesTo(_bot->state.food, 20);
    for(Route food : foodRoutes)
        if(!_bot->targets.ContainsKey(food.end) && !_bot->targets.ContainsValue(food.start))
            doMoveLocation(food.start, food.end, true);
    
    /////       ***** Exploration *****      /////
    // Explore close unseen areas
    exploration(_bot->state.viewRadius / 2.0f);

    // Moving out from our hills
    moveOutFromHills();
}
