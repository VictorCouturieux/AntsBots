#pragma once
#include "Behaviour.h"

/**
 * \brief Attack behaviour, prioritising to attack ants and hills
 */
class Conquest : public Behaviour
{
private:
    bool Initialised=false;
    /**
     * \brief Diamond location around our hills
     */
    vector<Location> Protectors; // Because even if we attack, we need to check for invaders TODO : Use these locations to protect our hills
public:
    Conquest(Bot* _bot) : Behaviour(_bot, Attack) {}
    void makeMoves() override;
    
    /**
     * \brief Build the Protectors vector, to set some defense
     */
    void Init() override;
};
