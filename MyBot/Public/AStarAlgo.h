#pragma once

#include "State.h"

using namespace std;

/**
 * \brief A* Search Algorithm to find the shortest path with euclidean heuristic
 */
class AStarAlgo
{
public:
    
    AStarAlgo(State& state) : _state(state){}

    /**
     * \brief Initializes a 2D grid of nodes whose size is equal to the game map
     */
    void SetupMap();
    
    /**
     * \brief Calculate the shorter path from antLoc to destLoc
     * \param antLoc Ant Location (Starting point)
     * \param destLoc Destination Location (Ending point)
     * \return Vector containing the path in the form of { [antLoc],...,[destLoc] }
     */
    vector<Location> GetPath(Location antLoc, Location destLoc);

private:
    State& _state;
    vector<vector<Node>> _baseGrid;
    
    /**
     * \brief Compute costs between CurrentNode and NeighbourNode,
     * updating the NeighbourNode costs if its path is shorter
     * \param CurrentNode The currently accessed node
     * \param NeighbourNode The neighbour of the current node
     */
    void computeCost(Node* CurrentNode, Node* NeighbourNode) const;
    
    /**
     * \brief Analyse and Update NeighbourNode if the path to it is shorter passing by the CurrentNode, and add it to the open list 
     * \param CurrentNode The currently accessed node
     * \param NeighbourNode The neighbour of the current node
     * \param openList List of nodes to be considered
     */
    void updateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList) const;
};


