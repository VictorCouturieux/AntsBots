#pragma once

#include "Behaviour.h"
#include "Bot.h"

class GatheringFood : public Behaviour
{
public:
    /**
     * \brief constructor create behaviour for ant to give it a food research at first
     * \param bot : bot used to get state info of map 
     */
    GatheringFood(Bot* bot) : Behaviour(bot, Food){}

    /**
     * \brief make move for ants and make different move to location.
     * at first found and go to food,
     * second, if the ant found a hill it go to destroy it,
     * third, the ant explore the unexplored map zone
     * fourth, the ant go on a random position on map
     */
    void MakeMoves() override;
};
