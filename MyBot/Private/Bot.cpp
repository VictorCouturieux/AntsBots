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

    Behaviours.insert(pair<GameState, Behaviour*>(Food, new GatheringFood(this)));
    Behaviours.insert(pair<GameState, Behaviour*>(Defense, new DefendHomeland(this)));
    Behaviours.insert(pair<GameState, Behaviour*>(Attack, new Conquest(this)));
    
    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

void Bot::checkAntPath()
{
    for (const auto& po : pathOrders.GetMap())
    {

        if (find(state.myAnts.begin(), state.myAnts.end(), po.first) == state.myAnts.end())
        {
            //state.bug << "erase" << endl;
            const auto it = pathOrders.GetRefMap().find(po.first);
            //state.bug << "it" << endl;
            pathOrders.GetRefMap().erase(pathOrders.GetRefMap().find(po.first));
        }
    }
}

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "" << endl;
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << "nb ant : " << state.myAnts.size() << endl;
    
    for (auto ant : state.myAnts)
    {
        state.bug << "[" << ant.row << ":" << ant.col << "] ";
    }
    state.bug << "" << endl;

    Behaviour* CurrentBehaviour = Behaviours.at(gameState);
    const int nbAnts = CurrentBehaviour->nbAnts;
    const int nbFood = CurrentBehaviour->nbFood;
/*
    switch(CurrentBehaviour->type)
    {
        case Food:
            if(nbAnts > 30) gameState = Attack;
            else if(nbAnts < 10) gameState = Defense;
            break;
        case Defense:
            if(nbAnts > 10) gameState = Food;
            break;
        case Attack:
            if(nbAnts < 30) gameState = Food;
            break;
    }*/
    
    CurrentBehaviour->makeMoves();
    
    state.bug << "nb pathOrders : " << pathOrders.GetMap().size() << endl;
    for (const auto& kv : pathOrders.GetMap()) {
        state.bug << "antPath => (" << kv.first.row << ":" << kv.first.col << ") values => ";
        for (Location value : kv.second)
            state.bug << value.row << ":" << value.col << " ";
        state.bug << endl;
    }
    if (state.myAnts.size() < pathOrders.GetMap().size())
        state.bug << "pathOrders OVER" << endl;
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
