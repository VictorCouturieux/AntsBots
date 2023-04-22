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
    state.bug << state << endl;

    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
        for(int d=0; d<TDIRECTIONS; d++)
            if(doMoveDirection(state.myAnts[ant], d))
                break;

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
    
}
