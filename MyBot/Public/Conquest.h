#pragma once
#include "Behaviour.h"

class Conquest : public Behaviour
{
private:
    bool Initialised=false;
    vector<Location> Protectors; // Because even if we attack, we need to check for invaders
public:
    Conquest(Bot* _bot) : Behaviour(_bot, Attack) {}
    void makeMoves() override;
    void Init() override;
};
