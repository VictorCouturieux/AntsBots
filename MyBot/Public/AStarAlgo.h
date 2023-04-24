﻿#pragma once

#include "State.h"

// A C++ Program to implement A* Search Algorithm
using namespace std;

// A structure to hold the necessary parameters
//
// struct Node
// {
//     int y;
//     int x;
//     int parentX;
//     int parentY;
//     float gCost;
//     float hCost; 
//     float fCost;
// };
//
// inline bool operator < (const Node& lhs, const Node& rhs)
// {//We need to overload "<" to put our struct into a set
//     return lhs.fCost < rhs.fCost;
// }


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
    void ComputeCost(Node* CurrentNode, Node* NeighbourNode);
    void UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList);
};

