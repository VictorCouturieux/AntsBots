#pragma once
#include "Behaviour.h"

/**
 * \brief Defense behaviour, prioritising to defend and trying to survive at all costs
 */
class DefendHomeland : public Behaviour
{
public:
    DefendHomeland(Bot* bot) : Behaviour(bot, Defense) {}
    void makeMoves() override;
};
