#pragma once
#include "Behaviour.h"

class GatheringFood : public Behaviour
{
public:
    GatheringFood(Bot* bot) : Behaviour(bot, Food) {} 
    void makeMoves() override;
};
