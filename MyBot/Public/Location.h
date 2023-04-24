#pragma once
#include <string>
#include <cfloat>
#include <limits.h>

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    std::string ToString() const
    {
        return "[" + std::to_string(row) + ", " + std::to_string(col) + "]";
    }
    // To use == operator
    bool operator==(const Location & other) const
    {
        return ( row == other.row && col == other.col );
    }
    // To use != operator
    bool operator!=(const Location & other) const
    {
        return ( row != other.row || col != other.col );
    }

    // To make std::map works
    bool operator<(const Location & other) const
    {
         if ( row < other.row )
             return true;
         else if ( row == other.row )
             return ( col < other.col );
         else
             return false;
    }
};

struct Node
{
    Location Position;
    Node* Parent;

    int gCost;
    float hCost, fCost;

    Node()
    {
        Position = Location(-1,-1);
        Parent = nullptr;
        gCost = INT_MAX;
        hCost = fCost = FLT_MAX;
    }
    
    std::string ToString() const
    {
        return "gCost : " + std::to_string(gCost) + " / hCost : " + std::to_string(hCost) + " / fCost : " + std::to_string(fCost) + " - Parent : " + Parent->Position.ToString();
    }
    
    // To make std::map works
    bool operator<(const Node & other) const
    {
        return fCost < other.fCost;
    }
    bool operator==(const Node & other) const
    {
        return Position == other.Position;
    }
};