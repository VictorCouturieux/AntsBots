#include "../Public/Behaviour.h"

#include <sstream>

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
    //bot->state.bug << "Test" << endl;

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
        //bot->state.bug << "MOVE " << antLoc.ToString()  << "->" << newLoc.ToString() << endl << endl;
        return true;
    }
    else
    return false;
}

bool Behaviour::doMoveLocation(const Location& antLoc, const Location& destLoc, bool pathFinding)
{
    if (find(bot->state.myAnts.begin(), bot->state.myAnts.end(), antLoc) != bot->state.myAnts.end())
    {
        Location nextMove;
        if(pathFinding) // && bot->state.directAccessTarget(antLoc, destLoc)
        {
            // init default empty value
            vector<Location> antPath;
            //get path if exist
            if (bot->pathOrders.containsKey(antLoc))
            {
                antPath = bot->pathOrders.GetMap().at(antLoc);
                bot->state.bug << "Path at " << antLoc.ToString() << endl; 
            }
            
            if(antPath.empty() || antPath.back() != destLoc)
            {
                if(!antPath.empty())
                    bot->state.bug << "WTF " << destLoc.ToString() << " " << antPath.back().ToString() << endl; 
                if (bot->pathOrders.containsKey(antLoc))
                {
                    //remove potential existing path
                    bot->pathOrders.erase(antLoc);
                }
                //Call A* function
                antPath = aStarPathFinding->aStar(antLoc, destLoc);
                if (antPath.size() > 0)
                {
                    //remove first step because it equals to Key
                    antPath.erase(antPath.begin());
                
                    //insert the new value
                    bot->pathOrders.insert(antLoc, antPath);
                }
            }
            //if (antPath.size() != 0) bot->state.bug << "bot->state.isFree(antPath[0])) = " << std::noboolalpha << bot->state.isFree(antPath[0]) << endl;
            if (antPath.size() != 0 && bot->state.isFree(antPath[0]))
            {
                //setup next step
                nextMove = Location(antPath[0].row, antPath[0].col);
                
                //set new location to Key.
                bot->pathOrders.updateKey(antLoc, nextMove);
                
                //remove the next step of path
                bot->pathOrders.GetRefMap()[nextMove].erase(bot->pathOrders.GetRefMap()[nextMove].begin());

                // check if path affected to a ant by position
                bot->checkAntPath();
                
            }
            else
            {
                nextMove = antLoc;
            }
        }
        else
            nextMove = destLoc;
        
        // Recover the closest directions to go from antLoc to destLoc
        array< int, 2 > directions;
        const int nbDirections = bot->state.getClosestDirections(antLoc, nextMove, directions);

        //bot->state.bug << nbDirections << " directions"  << endl;
        for (int i = 0; i < nbDirections; i++)
            if (doMoveDirection(antLoc, directions[i])) // Check collisions and do the move
            {
                bot->targets.insert(destLoc, antLoc);
                return true;
            }
    }
    return false;
}

void Behaviour::moveOutFromHills()
{
    //bot->state.bug << "moveOutFromHills" << endl;
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
