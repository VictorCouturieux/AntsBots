#pragma once
#include "Behaviour.h"

class Conquest : public Behaviour
{
public:
    Conquest(Bot* bot) : Behaviour(bot, Attack) {}
    void makeMoves() override;
};
