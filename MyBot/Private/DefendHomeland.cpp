#include "../Public/DefendHomeland.h"
#include "../Public/Bot.h"

void DefendHomeland::MakeMoves()
{
    Behaviour::MakeMoves();
    
    /////       *****     Defense     *****      /////
    // TODO : Everything
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes = getShortestRoutesTo(_bot->state.food, 20);
    for(Route food : foodRoutes)
        if(!_bot->targets.ContainsKey(food.end) && !_bot->targets.ContainsValue(food.start))
            doMoveLocation(food.start, food.end, true);
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    exploration(_bot->state.viewRadius / 2.0f);

    // Moving out from our hills
    moveOutFromHills();
}
