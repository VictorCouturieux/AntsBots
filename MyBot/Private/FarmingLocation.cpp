
#define _USE_MATH_DEFINES
#include "../Public/FarmingLocation.h"
#include <math.h>
#include <algorithm>

FarmingLocation::FarmingLocation(State& state) : state(state)
{
    int fieldSize = (int) (state.viewradius );//(int)(state.viewradius/sqrt(2));
    state.bug << "fieldSize = " << fieldSize << std::endl;

    for (int r = fieldSize; r < state.rows ; r += fieldSize)
        for (int c = fieldSize; c < state.cols ; c += fieldSize)
        {
            int row = r % state.rows;
            int col = c % state.cols;
            
            if (!state.grid[row][col].isWater)
                farmingList.push_back(Location(row, col));
            //else
                //findClosestLoc(Location(row, col));
            
        }
    
    state.bug << "pointsGenerated size = " << farmingList.size() << std::endl;
    state.bug << "pointsGenerated = > " ;
    for (Location loc : farmingList)
        state.bug << loc.ToString() ;
    state.bug << std::endl;
}

bool FarmingLocation::findClosestLoc(Location& loc)
{
    int searchStartX = loc.row-3 % state.rows; 
    int searchEndX = loc.row+3 % state.rows; 
        
    int searchStartY = loc.col-3 % state.cols;
    int searchEndY =  loc.col+3 % state.cols;

    std::vector<Route> possibleLoc;
    for (int x = searchStartX; x <= searchEndX; x++)
    {
        for (int y = searchStartY; y <= searchEndY; y++)
        {
            if (!state.grid[x][y].isWater)
            {
                possibleLoc.push_back(Route(loc, Location(x, y), state.ManhattanDistance(loc, Location(x, y))));
            }
        }
    }

     if (possibleLoc.empty()) return false;
     if (possibleLoc.size() != 1)
         std::sort( possibleLoc.begin(), possibleLoc.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
    loc.row = possibleLoc[0].End.row;
    loc.col = possibleLoc[0].End.col;
    //farmingList.push_back(possibleLoc[0].End);
    return true;
}
