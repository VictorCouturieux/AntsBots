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

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;

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
