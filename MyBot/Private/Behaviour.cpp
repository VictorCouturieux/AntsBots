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
    // check if path affected to a ant by position
    bot->checkAntPath();
    
    aStarPathFinding->setupMap();
    
    // Remove any tiles that can be seen
    for (int i = 0; i < bot->unseenLocations.size(); i++)
    {
        Location loc = bot->unseenLocations[i];
        if (bot->state.grid[loc.row][loc.col].isVisible)
            bot->unseenLocations.erase(bot->unseenLocations.begin() + i);
        if (bot->state.grid[loc.row][loc.col].isWater)
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
        
        // If we allow the ant to use the pathfinding
        if(pathFinding)
        {
            vector<Location> antPath;

            // If the path was already calculated, we recover it
            if (bot->pathOrders.containsKey(antLoc))
            {
                antPath = bot->pathOrders.GetMap().at(antLoc);
                // If the targeted location of the path is not the destLoc, it means that we changed path
                if (antPath.back() != destLoc)
                {
                    //remove potential existing path
                    bot->pathOrders.erase(antLoc);
                    antPath.clear();
                }
            }

            // If there is no path, or if the destination changed, we build a new path with A*
            if(antPath.empty())
            {
                //Call A* function
                antPath = aStarPathFinding->aStar(antLoc, destLoc);

                // TODO : Just return the path without the current antLoc
                // Remove the first step, which is equal to the current antLoc
                if (!antPath.empty())
                {
                    //remove first step because it equals to Key
                    antPath.erase(antPath.begin());
                
                    //insert the new value
                    bot->pathOrders.insert(antLoc, antPath);
                }
            }

            if (!antPath.empty() && bot->state.isFree(antPath[0]))
            {
                //setup next step
                nextMove = Location(antPath[0].row, antPath[0].col);
                
                //set new location to Key.
                bot->pathOrders.updateKey(antLoc, nextMove);
                
                //remove the next step of path
                bot->pathOrders.GetRefMap()[nextMove].erase(bot->pathOrders.GetRefMap()[nextMove].begin());
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

        for (int i = 0; i < nbDirections; i++)
            if (doMoveDirection(antLoc, directions[i])) // Check collisions and do the move
            {
                bot->targets.insert(destLoc, antLoc);
                return true;
            }
    }
    return false;
}

vector<Route> Behaviour::getShortestRoutesTo(vector<Location> destinations, double range)
{
    vector<Route> routes;
    for (Location destLoc : destinations)
        for(Location antLoc : bot->state.myAnts)
            if (!bot->orders.containsValue(antLoc))
            {
                const double distance = bot->state.EuclideanDistance(antLoc, destLoc);
                if(distance <= range)
                    routes.emplace_back(antLoc, destLoc, distance);
            }
    // Sort the routes list in a way that we have the shortests distances first
    sort( routes.begin(), routes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    return routes;
}

void Behaviour::Exploration(float range)
{
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
                        if(distance <= range)
                            unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
                    }
                }

                // Sort to have the lower distances first
                sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

                for (Route route : unseenRoutes)
                {
                    if(doMoveLocation(route.Start, route.End, true))
                    {
                        break;
                    }
                }
            }  
        }
    }
}

void Behaviour::moveOutFromHills()
{
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
