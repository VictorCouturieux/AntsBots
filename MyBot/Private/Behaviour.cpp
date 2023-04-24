#include "../Public/Behaviour.h"

#include "../Public/AStarAlgo.h"
#include "../Public/Bot.h"

Behaviour::Behaviour(Bot* _bot, GameState _type)
{
    bot = _bot;
    type = _type;
    nbFood = _bot->state.food.size();
    nbAnts = _bot->state.myAnts.size();
    aStarPathFinding = new AStarAlgo(_bot->state);
}

void Behaviour::makeMoves()
{
    bot->state.bug << "Test" << endl;

    aStarPathFinding->setupMap();
    
    // Remove any tiles that can be seen
    for (int i = 0; i < bot->unseenLocations.size(); i++)
    {
        Location loc = bot->unseenLocations[i];
        if (bot->state.grid[loc.row][loc.col].isVisible)
            bot->unseenLocations.erase(bot->unseenLocations.begin() + i);
    }
    
    // Reset the last turn maps
    bot->orders.clear();
    bot->targets.clear();

    nbFood = bot->state.food.size();
    nbAnts = bot->state.myAnts.size();
    
    // Prevent Stepping on own hill
    for(const Location hill : bot->state.myHills)
        bot->orders.insert(hill, Location());
}

bool Behaviour::doMoveDirection(const Location& antLoc, int dir)
{
    Location newLoc = bot->state.getLocation(antLoc, dir);
    if (bot->state.isFree(newLoc) && !bot->orders.containsKey(newLoc))
    {
        bot->state.makeMove(antLoc, dir);
        bot->orders.insert(newLoc, antLoc);
        bot->state.bug << "MOVE " << antLoc.ToString()  << "->" << newLoc.ToString() << endl << endl;
        return true;
    }
    else
    return false;
}

bool Behaviour::doMoveLocation(const Location& antLoc, const Location& destLoc, bool pathFinding)
{
    Location nextMove;
    if(pathFinding)
    {
        bot->state.bug << "Call A* function" << endl;
        vector<Location> path = aStarPathFinding->aStar(antLoc, destLoc);
        bot->state.bug << "Pathfinding size : " << path.size() << endl;
        bot->state.bug << "... Go to " << path[1].ToString() << endl << endl;
        nextMove = path[1];
    }
    else
        nextMove = destLoc;
    
    // Recover the closest directions to go from antLoc to destLoc
    array< int, 2 > directions;
    const int nbDirections = bot->state.getClosestDirections(antLoc, nextMove, directions);

    bot->state.bug << nbDirections << " directions"  << endl;
    for (int i = 0; i < nbDirections; i++)
        if (doMoveDirection(antLoc, directions[i])) // Check collisions and do the move
            {
            bot->targets.insert(destLoc, antLoc);
            return true;
            }

    return false;
}

void Behaviour::moveOutFromHills()
{
    bot->state.bug << "moveOutFromHills" << endl;
    for(const Location hillLoc : bot->state.myHills)
        // If there is an ant above a hill
        if(std::find(bot->state.myAnts.begin(), bot->state.myAnts.end(), hillLoc) != bot->state.myAnts.end())
        {
            // Check if the ant is already moving (ie if an order is already attributed to the ant on hill)
            bool alreadyMoving=false;
            if(bot->orders.containsValue(hillLoc))
                alreadyMoving = true;

            // If not, we try to move it anyway
            if(!alreadyMoving)
            {
                for(int d=0; d < TDIRECTIONS; d++)
                    if(doMoveDirection(hillLoc, d))
                        break;                
            }
        }
}
