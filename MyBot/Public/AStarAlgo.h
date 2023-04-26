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
    
    float calculateH(int row, int col, Location dest);
    float calculateManhattanH(int row, int col, Location dest);
    void ComputeCost(Node* CurrentNode, Node* NeighbourNode);
    void UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList);
};


