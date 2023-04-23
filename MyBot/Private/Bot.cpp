#include "../Public/Bot.h"

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();
    
    // add all locations to unseen tiles set, run once
    if (unseenLocations.empty())
        for (int row = 0; row < state.rows; row ++)
            for (int col = 0; col < state.cols; col ++)
                unseenLocations.push_back(Location(row, col));
    
    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    //state.bug << state << endl;
    
    // remove any tiles that can be seen, run each turn
    for (int i = 0; i < unseenLocations.size(); i++)
    {
        Location loc = unseenLocations[i];
        if (state.grid[loc.row][loc.col].isVisible)
        {
            unseenLocations.erase(unseenLocations.begin() + i);
        }
    }

    int nbFood = state.food.size();
    int nbAnts = state.myAnts.size();
    // Reset the last turn maps
    orders.clear();
    targets.clear();

    // Prevent Stepping on own hill
    for(const Location hill : state.myHills)
        orders.insert(pair< Location, Location >(hill, Location()));
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes( nbFood*nbAnts);
    int ID=0;
    for(Location food : state.food)
        for(Location ant : state.myAnts)
        {
            const double distance = state.distance(ant, food);
            foodRoutes[ID++] = Route(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    for(Route food : foodRoutes)
        if(!targets.containsKey(food.End) && !targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End);
    
    /*
    //Default move
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
        for(int d=0; d<TDIRECTIONS; d++)
            if(doMoveDirection(state.myAnts[ant], d))
                break;
    */

    /////       ***** Attacking ennemies *****      /////
    // add new hills to set
    for (Location enemyHill : state.enemyHills)
        if (std::find(enemyHillsFounded.begin(), enemyHillsFounded.end(), enemyHill) == enemyHillsFounded.end())
            enemyHillsFounded.push_back(enemyHill);

    // attack hills
    vector<Route> hillRoutes( nbAnts*enemyHillsFounded.size());
    ID=0;
    for (Location hillLoc : enemyHillsFounded)
        for(Location ant : state.myAnts)
            if (orders.find(ant) == orders.end())
            {
                const double distance = state.distance(ant, hillLoc);
                hillRoutes[ID++] = Route(ant, hillLoc, distance);
            }
    sort( hillRoutes.begin(), hillRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
    for (Route route : hillRoutes)
        doMoveLocation(route.Start, route.End);
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    for(Location antLoc : state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (orders.find(antLoc) == orders.end())
        {
            vector<Route> unseenRoutes;
            for (Location unseenLoc  : unseenLocations)
            {
                const double distance = state.distance(antLoc, unseenLoc);
                unseenRoutes.push_back(Route(antLoc, unseenLoc, distance));
            }
            sort( unseenRoutes.begin(), unseenRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
            for (Route route : unseenRoutes)
                doMoveLocation(route.Start, route.End);
        }
    }

    // Moving out from our hills
    for(const Location hillLoc : state.myHills)
        // If there is an ant above a hill
        if(std::find(state.myAnts.begin(), state.myAnts.end(), hillLoc) != state.myAnts.end())
        {
            // Check if the ant is already moving (ie if an order is already attributed to the ant on hill)
            bool alreadyMoving=false;
            for(const auto& it: orders)
                if(it.second == hillLoc)
                {
                    alreadyMoving = true;
                    break;
                }

            // If not, we try to move it anyway
            if(!alreadyMoving)
            {
                for(int d=0; d < TDIRECTIONS; d++)
                    if(doMoveDirection(hillLoc, d))
                        break;                
            }
        }
    
    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};

bool Bot::doMoveDirection(const Location &antLoc, int dir)
{
    Location newLoc = state.getLocation(antLoc, dir);
    if (state.isFree(newLoc) && orders.find(newLoc) == orders.end())
    {
        state.makeMove(antLoc, dir);
        orders.insert( pair< Location, Location >(newLoc, antLoc) );
        return true;
    }
    return false;
}

bool Bot::doMoveLocation(const Location& antLoc, const Location& destLoc)
{
    // Recover the closest directions to go from antLoc to destLoc
    array< int, 2 > directions;
    const int nbDirections = state.getClosestDirections(antLoc, destLoc, directions);

    for (int i = 0; i < nbDirections; i++)
        if (doMoveDirection(antLoc, directions[i])) // Check collisions and do the move
        {
            targets.insert(destLoc, antLoc);
            return true;
        }

    return false;
}
