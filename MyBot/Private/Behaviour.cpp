#include "../Public/Behaviour.h"

#include <sstream>

#include "../Public/AStarAlgo.h"
#include "../Public/Bot.h"

Behaviour::Behaviour(Bot* _bot, GameState _type)
{
    this->_bot = _bot;
    this->type = _type;
    this->nbFood = _bot->state.food.size();
    this->nbAnts = _bot->state.myAnts.size();
    this->_pathFinding = new AStarAlgo(_bot->state);
}

void Behaviour::MakeMoves()
{
    // check if path affected to a ant by position
    _bot->CheckAntPath();
    
    _pathFinding->SetupMap();
    
    // Remove any tiles that can be seen
    for (int i = 0; i < _bot->unseenLocations.size(); i++)
    {
        Location loc = _bot->unseenLocations[i];
        if (_bot->state.grid[loc.row][loc.col].isVisible)
            _bot->unseenLocations.erase(_bot->unseenLocations.begin() + i);
        if (_bot->state.grid[loc.row][loc.col].isWater)
            _bot->unseenLocations.erase(_bot->unseenLocations.begin() + i);
    }
    
    // Reset the last turn maps
    _bot->orders.Clear();
    _bot->targets.Clear();

    nbFood = _bot->state.food.size();
    nbAnts = _bot->state.myAnts.size();
    
    // Prevent Stepping on own hill
    for(const Location hill : _bot->state.myHills)
        _bot->orders.Insert(hill, Location());
}

bool Behaviour::doMoveDirection(Location antLoc, int dir) const
{
    Location newLoc = _bot->state.GetLocation(antLoc, dir);
    if (_bot->state.IsFree(newLoc) && !_bot->orders.ContainsKey(newLoc))
    {
        _bot->state.MakeMove(antLoc, dir);
        _bot->orders.Insert(newLoc, antLoc);
        return true;
    }
    return false;
}

bool Behaviour::doMoveLocation(Location antLoc, Location destLoc, bool pathFinding) const
{
    if (find(_bot->state.myAnts.begin(), _bot->state.myAnts.end(), antLoc) != _bot->state.myAnts.end())
    {
        Location nextMove;
        
        // If we allow the ant to use the pathfinding
        if(antLoc != destLoc && pathFinding)
        {
            vector<Location> antPath;

            // If the path was already calculated, we recover it
            if (_bot->pathOrders.ContainsKey(antLoc))
            {
                antPath = _bot->pathOrders.GetMap().at(antLoc);
                // If the targeted location of the path is not the destLoc, it means that we changed path
                if (antPath.back() != destLoc)
                {
                    //remove potential existing path
                    _bot->pathOrders.Erase(antLoc);
                    antPath.clear();
                }
            }

            // If there is no path, or if the destination changed, we build a new path with A*
            if(antPath.empty())
            {
                //Call A* function
                antPath = _pathFinding->GetPath(antLoc, destLoc);

                // Remove the first step, which is equal to the current antLoc
                if (!antPath.empty())
                {
                    //remove first step because it equals to Key
                    antPath.erase(antPath.begin());
                
                    //insert the new value
                    _bot->pathOrders.Insert(antLoc, antPath);
                }
            }

            // Update the pathOrder, erasing the first element which is the ant location before moving
            if (!antPath.empty() && _bot->state.IsFree(antPath[0]))
            {
                //setup next step
                nextMove = Location(antPath[0].row, antPath[0].col);
                
                //set new location to Key.
                _bot->pathOrders.UpdateKey(antLoc, nextMove);
                
                //remove the first step of path
                _bot->pathOrders.GetRefMap()[nextMove].erase(_bot->pathOrders.GetRefMap()[nextMove].begin());
            }
            // If it can't move, stay static
            else
                nextMove = antLoc;
        }
        else
            nextMove = destLoc;
        
        // Recover the closest directions to go from antLoc to destLoc
        array< int, 2 > directions;
        const int nbDirections = _bot->state.GetClosestDirections(antLoc, nextMove, directions);

        for (int i = 0; i < nbDirections; i++)
            if (doMoveDirection(antLoc, directions[i])) // Check collisions and do the move
            {
                _bot->targets.Insert(destLoc, antLoc);
                return true;
            }
    }
    return false;
}

vector<Route> Behaviour::getShortestRoutesTo(vector<Location> destinations, double range) const
{
    vector<Route> routes;
    for (Location destLoc : destinations)
        for(Location antLoc : _bot->state.myAnts)
            if (!_bot->orders.ContainsValue(antLoc))
            {
                const double distance = _bot->state.EuclideanDistance(antLoc, destLoc);
                if(distance <= range)
                    routes.emplace_back(antLoc, destLoc, distance);
            }
    // Sort the routes list in a way that we have the shortests distances first
    sort( routes.begin(), routes.end(), [](Route a, Route b) { return a.distance < b.distance; } );

    return routes;
}

void Behaviour::exploration(float range) const
{
    for(Location antLoc : _bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!_bot->orders.ContainsValue(antLoc))
        {
            // If the ant was already previously exploring, we recover its destination and start the movement again
            if(_bot->pathOrders.ContainsKey(antLoc))
            {
                Location destLoc = _bot->pathOrders.GetMap()[antLoc][_bot->pathOrders.GetMap()[antLoc].size()-1];
                doMoveLocation(antLoc, destLoc);
            }
            
            // Otherwise, we'll want to build a new exploring path
            else
            {
                vector<Route> unseenRoutes;
                for (Location unseenLoc  : _bot->unseenLocations)
                {
                    // If the targeted unseen location is water, we remove it from the vector, as destination is unreachable
                    if(_bot->state.grid[unseenLoc.row][unseenLoc.col].isWater)
                        remove(_bot->unseenLocations.begin(), _bot->unseenLocations.end(), unseenLoc);

                    // Register the location as a route if the destination is not too far
                    else
                    {
                        const double distance = _bot->state.ManhattanDistance(antLoc, unseenLoc);
                        if(distance <= range)
                            unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
                    }
                }

                // Sort to have the lower distances first
                sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.distance < b.distance; } );

                for (Route route : unseenRoutes)
                    if(doMoveLocation(route.start, route.end, true))
                        break;
            }  
        }
    }
}

void Behaviour::moveOutFromHills() const
{
    for(const Location hillLoc : _bot->state.myHills)
        // If there is an ant above a hill
        if(std::find(_bot->state.myAnts.begin(), _bot->state.myAnts.end(), hillLoc) != _bot->state.myAnts.end())
        {
            // Check if the ant is already moving (ie if an order is already attributed to the ant on hill)
            bool alreadyMoving=false;
            if(_bot->orders.ContainsValue(hillLoc))
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
