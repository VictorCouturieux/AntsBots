#pragma once


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

    // To use == operator
    bool operator==(const Location & other) const
    {
        return ( row == other.row && col == other.col );
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
