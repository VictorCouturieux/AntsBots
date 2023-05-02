#include "../Public/Bot.h"

using namespace std;

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

    // insert behaviours in behavioursList 
    Behaviours.insert(pair<GameState, Behaviour*>(Food, new GatheringFood(this)));
    Behaviours.insert(pair<GameState, Behaviour*>(Defense, new DefendHomeland(this)));
    Behaviours.insert(pair<GameState, Behaviour*>(Attack, new Conquest(this)));
    
    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        if(state.turn == 1)
            for(auto behaviour : Behaviours)
                behaviour.second->Init();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    // get infos of ants number and food number see by ants 
    Behaviour* CurrentBehaviour = Behaviours.at(gameState);
    const int nbAnts = CurrentBehaviour->nbAnts;
    const int nbFood = CurrentBehaviour->nbFood;

    // switch behaviour according to number of ants
    switch(CurrentBehaviour->type)
    {
        case Food:
            if(nbAnts > 30) gameState = Attack;
            //// Uncomment this to allow to go to defend state, when it will be done
            //else if(nbAnts < 10) gameState = Defense;
            break;
        case Defense:
            if(nbAnts > 10) gameState = Food;
            break;
        case Attack:
            if(nbAnts < 30) gameState = Food;
            break;
    }
    
    CurrentBehaviour->makeMoves();
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};


void Bot::checkAntPath()
{
    //foreach Path Orders
    for (const auto& po : pathOrders.GetMap())
    {
        //if the location list is empty, remove it
        // or if the ant reference in the path orders doesn't exist, remove it
        if (po.second.empty() || find(state.myAnts.begin(), state.myAnts.end(), po.first) == state.myAnts.end())
        {
            pathOrders.erase(po.first);
        }
    }
}
