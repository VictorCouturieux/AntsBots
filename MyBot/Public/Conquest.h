#pragma once
#include "Behaviour.h"

/**
 * \brief Attack behaviour, prioritising to attack ants and hills
 */
class Conquest : public Behaviour
{
private:
    bool _initialised=false;
    /**
     * \brief Diamond location around our hills
     */
    vector<Location> _protectors; // Because even if we attack, we need to check for invaders TODO : Use these locations to protect our hills
public:
    Conquest(Bot* _bot) : Behaviour(_bot, Attack) {}
    void MakeMoves() override;
    
    /**
     * \brief Build the Protectors vector, to set some defense
     */
    void Init() override;
};
