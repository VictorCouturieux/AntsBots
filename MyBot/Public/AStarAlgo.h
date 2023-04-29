#pragma once

#include "State.h"

// A C++ Program to implement A* Search Algorithm
using namespace std;

class AStarAlgo
{
public:
    State& state;
    
    AStarAlgo(State& state) : state(state){}
    
    void setupMap();
    vector<Location> aStar(Location antLoc, Location dest);

private:
    vector<vector<Node>> baseGrid;
    
    void ComputeCost(Node* CurrentNode, Node* NeighbourNode);
    void UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList);
};


