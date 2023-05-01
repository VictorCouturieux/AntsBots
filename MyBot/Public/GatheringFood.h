#pragma once

#include "FarmingLocation.h"
#include "Behaviour.h"
#include "Bot.h"

class GatheringFood : public Behaviour
{
public:
    FarmingLocation* farmLocs;
    
    GatheringFood(Bot* bot) : Behaviour(bot, Food){} 
    void makeMoves() override;
    void Init() override;
};
