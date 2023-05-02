#pragma once


#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"

/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

/**
 * \brief Behaviours key enum
 */
enum GameState { Attack, Defense, Food };

/**
 * \brief Struct to store current state information
 */
struct State
{
public:
    int rows, cols,
        turn, turns,
        noPlayers;
    double attackRadius, spawnRadius, viewRadius;
    double loadTime, turnTime;
    std::vector<double> scores;
    bool gameOver;
    int64_t seed;

    std::vector<std::vector<Square> > grid;
    std::vector<Location> myAnts, enemyAnts, myHills, enemyHills, food;

    Timer timer;
    Bug bug;

    State();
    ~State();

    /**
     * \brief sets the state up
     */
    void Setup();
 
    /**
     * \brief resets all non-water squares to land and clears the bots ant vector
     */
    void Reset();

    /**
     * \brief outputs move information to the engine
     * \param loc The location of the ant who will make move
     * \param direction Direction where the the ant move
     */
    void MakeMove(const Location &loc, int direction);

    /**
     * \brief returns the euclidean distance between two locations with the edges wrapped
     * \param loc1 location to begin euclidean algo
     * \param loc2 location to end euclidean algo
     * \return euclidean distance
     */
    double EuclideanDistance(const Location &loc1, const Location &loc2);

    /**
     * \brief returns the Manhattan distance between two locations with the edges wrapped
     * \param loc1 location to begin Manhattan algo
     * \param loc2 location to end Manhattan algo
     * \return Manhattan distance
     */
    double ManhattanDistance(const Location &loc1, const Location &loc2);

    /**
     * \brief returns the new location from moving in a given direction with the edges wrapped
     * \param startLoc Location reference where we set direction to found new location
     * \param direction direction where we get location
     * \return location we get
     */
    Location GetLocation(const Location &startLoc, int direction);
    
    /**
     * \brief search if ant can access to target without be blocked by wall.
     * \param loc1 ant location
     * \param loc2 searched destination 
     * \return if the way isn't bocked by water return true
     */
    bool DirectAccessTarget(const Location &loc1, const Location &loc2);

    void UpdateVisionInformation();
    
    /**
     * \brief Check whether the location is free or not
     * Check if a location is occupied by ant or water
     * \param loc location we checked
     * \return if the location isn't ant or water return true
     */
    bool IsFree(const Location & loc);
    
    /** 
     * \brief Takes a location and a destination and returns a list of the closest direction "as the crow flies", returning its directions array size (max:2)
     * \param antLoc started Ant location  
     * \param destLoc searched destination 
     * \param directions REFERENCE of one or two directions found to go to destLoc
     * \return directions number
     */
    int GetClosestDirections(const Location &antLoc, const Location &destLoc, std::array< int, 2 > & directions);
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);

