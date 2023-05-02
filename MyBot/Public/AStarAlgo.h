#pragma once

#include "State.h"

// A C++ Program to implement A* Search Algorithm
using namespace std;

class AStarAlgo
{
public:
    State& state;
    
    AStarAlgo(State& state) : state(state){}

    /**
     * \brief Initializes a 2D grid of nodes whose size is equal to the game map
     */
    void setupMap();
    
    /**
     * \brief Calculate the shorter path from antLoc to destLoc
     * \param antLoc Ant Location (Starting point)
     * \param destLoc Destination Location (Ending point)
     * \return Vector containing the path in the form of { [antLoc],...,[destLoc] }
     */
    vector<Location> aStar(Location antLoc, Location destLoc);

private:
    vector<vector<Node>> baseGrid;
    
    /**
     * \brief Compute costs between CurrentNode and NeighbourNode,
     * updating the NeighbourNode costs if its path is shorter
     * \param CurrentNode The currently accessed node
     * \param NeighbourNode The neighbour of the current node
     */
    void ComputeCost(Node* CurrentNode, Node* NeighbourNode) const;
    
    /**
     * \brief Analyse and Update NeighbourNode if the path to it is shorter passing by the CurrentNode, and add it to the open list 
     * \param CurrentNode The currently accessed node
     * \param NeighbourNode The neighbour of the current node
     * \param openList List of nodes to be considered
     */
    void UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList) const;
};


