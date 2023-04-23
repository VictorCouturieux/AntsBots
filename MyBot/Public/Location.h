#pragma once
#include <string>

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col, parentX, parentY;
    
    float gCost, hCost, fCost;

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
        return fCost < other.fCost;
        // if ( row < other.row )
        //     return true;
        // else if ( row == other.row )
        //     return ( col < other.col );
        // else
        //     return false;
    }
};