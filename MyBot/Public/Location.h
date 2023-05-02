#pragma once
#include <string>
#include <cfloat>
#include <limits.h>

/**
 * \brief Struct for representing locations in the grid.
 */
struct Location
{
    int row, col;

    Location() { row = col = 0; };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    /**
     * \brief Convert the Location to a string, for debug purposes
     */
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

    // To use + operator
    Location operator+(const Location & other) const
    {
       return Location(row + other.row, col + other.col);
    }
    
    // To use - operator
    Location operator-(const Location & other) const
    {
        return Location(row - other.row, col - other.col);
    }
};

/**
 * \brief Struct for representing A* Node in the grid.
 */
struct Node
{
    Location position;
    Node* parent;

    int gCost;
    float hCost, fCost;

    Node()
    {
        position = Location(-1,-1);
        parent = nullptr;
        gCost = INT_MAX;
        hCost = fCost = FLT_MAX;
    }
    
    /**
     * \brief Convert the Node to a string, for debug purposes
     */
    std::string ToString() const
    {
        return "gCost : " + std::to_string(gCost) + " / hCost : " + std::to_string(hCost) + " / fCost : " + std::to_string(fCost) + " - Parent : " + parent->position.ToString();
    }
    
    // To make std::map works
    bool operator<(const Node & other) const
    {
        return fCost < other.fCost;
    }
    // To use == operator
    bool operator==(const Node & other) const
    {
        return position == other.position;
    }
};