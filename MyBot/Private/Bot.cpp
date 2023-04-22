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

    int nbFood = state.food.size();
    int nbAnts = state.myAnts.size();
    // Reset the last turn maps
    orders.clear();
    targets.clear();
    
    // Food gathering
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
        if(!targets.containsKey(food.End))
            if(!targets.containsValue(food.Start))
                doMoveLocation(food.Start, food.End);
    /*
    //Default move
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
        for(int d=0; d<TDIRECTIONS; d++)
            if(doMoveDirection(state.myAnts[ant], d))
                break;
    */
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
