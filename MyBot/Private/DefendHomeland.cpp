#include "../Public/DefendHomeland.h"
#include "../Public/Bot.h"

void DefendHomeland::makeMoves()
{
    Behaviour::makeMoves();
    
    /////       *****     Defense     *****      /////
    // TODO : Everything
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes = getShortestRoutesTo(bot->state.food, 20);
    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    Exploration(bot->state.viewradius / 2.0f);

    // Moving out from our hills
    moveOutFromHills();
}
