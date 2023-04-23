#pragma once
#include "Behaviour.h"

class DefendHomeland : public Behaviour
{
public:
    DefendHomeland(Bot* bot) : Behaviour(bot, Defense) {}
    void makeMoves() override;
};
