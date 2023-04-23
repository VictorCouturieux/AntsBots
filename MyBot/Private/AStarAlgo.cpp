#include "../Public/AStarAlgo.h"



bool AStarAlgo::isDestination(int row, int col, Location dest) {
    if (row == dest.row && col == dest.col) {
        return true;
    }
    return false;
}

double AStarAlgo::calculateH(int row, int col, Location dest) {
    return sqrt((row - dest.row)*(row - dest.row)
            + (col - dest.col)*(col - dest.col));
}

vector<Location> AStarAlgo::makePath(vector<vector<Location>> map, Location dest) {
    vector<Location> empty;
    try {
        cout << "Found a path" << endl;
        int row = dest.row;
        int col = dest.col;
        stack<Location> path;
        vector<Location> usablePath;

        while (!(map[row][col].parentX == row && map[row][col].parentY == col)
            && map[row][col].row != -1 && map[row][col].col != -1) 
        {
            path.push(map[row][col]);
            int tempX = map[row][col].parentX;
            int tempY = map[row][col].parentY;
            row = tempX;
            col = tempY;

        }
        path.push(map[row][col]);

        while (!path.empty()) {
            Location top = path.top();
            path.pop();
            usablePath.emplace_back(top);
        }
        return usablePath;
    }
    catch(const exception& e){
        cout << e.what() << endl;
    }
    return empty;
}

void AStarAlgo::setupMap()
{
    //Initialize whole map
    allMap.resize(state.rows, vector<Location>(state.cols, Location()));
    for (int x = 0; x < state.rows; x++) {
        for (int y = 0; y < state.cols; y++) {
            allMap[x][y].fCost = FLT_MAX;
            allMap[x][y].gCost = FLT_MAX;
            allMap[x][y].hCost = FLT_MAX;
            allMap[x][y].parentX = -1;
            allMap[x][y].parentY = -1;
            allMap[x][y].row = x;
            allMap[x][y].col = y;
        }
    }
}

vector<Location> AStarAlgo::aStar(Location antLoc, Location dest) {
    vector<Location> empty;
    
    vector<vector<bool>> closedList(state.rows, vector<bool>(state.cols, false));
    
    //Initialize our starting list
    int x = antLoc.row;
    int y = antLoc.col;
    allMap[x][y].fCost = 0.0;
    allMap[x][y].gCost = 0.0;
    allMap[x][y].hCost = 0.0;
    allMap[x][y].parentX = x;
    allMap[x][y].parentY = y;
    
    vector<Location> openList;  
    openList.emplace_back(allMap[x][y]);
    bool destinationFound = false;
    
    while (! openList.empty() && (int)openList.size() < state.rows * state.cols) {
        Location node;
        do {
            //This do-while loop could be replaced with extracting the first
            //element from a set, but you'd have to make the openList a set.
            //To be completely honest, I don't remember the reason why I do
            //it with a vector, but for now it's still an option, although
            //not as good as a set performance wise.
            float temp = FLT_MAX;
            vector<Location>::iterator itNode;
            for (vector<Location>::iterator it = openList.begin();
                it != openList.end(); it = next(it)) {
                Location n = *it;
                if (n.fCost < temp) {
                    temp = n.fCost;
                    itNode = it;
                }
            }
            node = *itNode;
            openList.erase(itNode);
        } while (!state.grid[node.row][node.col].isWater);
    
        x = node.row;
        y = node.col;
        closedList[x][y] = true;
    
        //For each neighbour starting from North-West to South-East
        for (int newX = -1; newX <= 1; newX++) {
            for (int newY = -1; newY <= 1; newY++) {
                float gNew, hNew, fNew;
                if (state.grid[x + newX][y + newY].isWater) {
                    if (isDestination(x + newX, y + newY, dest))
                    {
                        //Destination found - make path
                        allMap[x + newX][y + newY].parentX = x;
                        allMap[x + newX][y + newY].parentY = y;
                        destinationFound = true;
                        return makePath(allMap, dest);
                    }
                    else if (closedList[x + newX][y + newY] == false)
                    {
                        gNew = (float) node.gCost + 1.0f;
                        hNew = (float) calculateH(x + newX, y + newY, dest);
                        fNew = gNew + hNew;
                        // Check if this path is better than the one already present
                        if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
                            allMap[x + newX][y + newY].fCost > fNew)
                        {
                            // Update the details of this neighbour node
                            allMap[x + newX][y + newY].fCost = fNew;
                            allMap[x + newX][y + newY].gCost = gNew;
                            allMap[x + newX][y + newY].hCost = hNew;
                            allMap[x + newX][y + newY].parentX = x;
                            allMap[x + newX][y + newY].parentY = y;
                            openList.emplace_back(allMap[x + newX][y + newY]);
                        }
                    }
                }
            }
        }
    }
    if (destinationFound == false) {
        state.bug << "Destination not found" << endl;
        return empty;
    }
    return empty;
}