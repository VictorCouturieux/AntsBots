#include "../Public/AStarAlgo.h"

#include <map>
#include <algorithm>

void AStarAlgo::setupMap()
{
    baseGrid.resize(state.rows, vector<Node>(state.cols, Node()));
    for (int x = 0; x < state.rows; x++) 
        for (int y = 0; y < state.cols; y++) 
            baseGrid[x][y].Position = Location(x,y);
}

void AStarAlgo::ComputeCost(Node* CurrentNode, Node* NeighbourNode) const
{
    // Manhattan distance between the neighbour
    int x = abs(CurrentNode->Position.row - NeighbourNode->Position.row),
        y = abs(CurrentNode->Position.col - NeighbourNode->Position.col);
    // Borders
    if(x > 2) x -= state.rows-2;
    if(y > 2) y -= state.cols-2;
    
    int cost = x + y;
    // If this path will be shorter to access the neighbour node, we update its costs
    if(CurrentNode->gCost + cost < NeighbourNode->gCost)
    {
        // Set Neighbour node's parent as the current one
        NeighbourNode->Parent = CurrentNode;
        // Update Neighbour node's cost
        NeighbourNode->gCost = CurrentNode->gCost + cost;
        NeighbourNode->fCost = NeighbourNode->gCost + NeighbourNode->hCost;
    }
}

void AStarAlgo::UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList) const
{
    const double oldCost = NeighbourNode->gCost;
    ComputeCost(CurrentNode, NeighbourNode);
    
    // If passing by the CurrentNode gives a shorter path to the NeighbourNode, we add it to the openList
    if(NeighbourNode->gCost < oldCost)
    {
        const auto it = find(openList.begin(), openList.end(), NeighbourNode);
        if(it != openList.end()) openList.erase(it);  
        openList.push_back(NeighbourNode);
    }       
}

vector<Location> AStarAlgo::aStar(Location antLoc, Location destLoc) {
    vector<Location> path;

    vector<vector<Node>> grid = baseGrid;

    // Calculate h(x) with destLoc
    for (int x = 0; x < state.rows; x++) 
        for (int y = 0; y < state.cols; y++) 
            grid[x][y].hCost = state.ManhattanDistance(Location(x,y), destLoc); // TODO : check si c'est pas mieux euclide
    
    // Init both open (nodes to be considered) and closed (nodes to ignore) lists
    vector<Node*> closedList;
    vector<Node*> openList;

    // Start position initialisation
    grid[antLoc.row][antLoc.col].gCost = 0;
    grid[antLoc.row][antLoc.col].fCost = grid[antLoc.row][antLoc.col].hCost;
    grid[antLoc.row][antLoc.col].Parent = &grid[antLoc.row][antLoc.col];
    openList.push_back(&grid[antLoc.row][antLoc.col]);
    
    while(!openList.empty())
    {        
        // Sort the array in a way to have the lowest fCost in last position
        sort( openList.begin(), openList.end(), [](Node* a, Node* b) { return a->fCost > b->fCost; } );
        // Pop the last Node in openList, and mark it as already checked by adding it to the closed list
        Node* CurrentNode = openList.back();
        openList.pop_back();
        closedList.push_back(CurrentNode);

        // If we found the destination
        if(CurrentNode->Position == destLoc)
        {
            // Build the path by seeking for Start Location from parents
            path.push_back(CurrentNode->Position);
            while(CurrentNode->Position != antLoc)
            {
                CurrentNode = CurrentNode->Parent;
                Location CurrentLocation = Location(CurrentNode->Position);

                // If the Current Location has two directions from the last step, we need to transform it as a Manhattan movement
                array< int, 2 > directions;
                if (!path.empty() && state.getClosestDirections(path[path.size()-1], CurrentLocation, directions) == 2)
                {
                    Location loc = state.getLocation(antLoc, directions[0]);
                    if (!state.grid[loc.row][loc.col].isWater)
                    {
                        path.push_back(state.getLocation(path[path.size()-1], directions[0]));
                        path.push_back(state.getLocation(path[path.size()-1], directions[1]));
                    } else
                    {
                        path.push_back(state.getLocation(path[path.size()-1], directions[1]));
                        path.push_back(state.getLocation(path[path.size()-1], directions[0]));
                    }
                }
                // Otherwise, we just need to push the only direction possible 
                else
                    path.push_back(CurrentLocation);
            }

            // Reverse the vector to have the Start point first
            std::reverse(path.begin(),path.end());
            break;
        }

        // Otherwise, we analyse its neighbours
        for(int x=-1; x <= 1; x++)
            for(int y=-1; y <= 1; y++)
            {
                int posX = CurrentNode->Position.row + x;
                int posY = CurrentNode->Position.col + y;
                // Check for Neighbours that are outside of map borders
                if(posX >= state.rows) posX = 0;
                else if(posX < 0) posX = state.rows-1;
                if(posY >= state.cols) posY = 0;
                else if(posY < 0) posY = state.cols-1;
                
                Node* NeighbourNode = &grid[posX][posY];

                // Firstly, if we are analysing a diagonal, we have to check if both of col and row nodes are water tiles
                if (abs(x) + abs(y) == 2)
                {
                    Node* RowNode = &grid[CurrentNode->Position.row][posY];
                    Node* ColNode = &grid[posX][CurrentNode->Position.col];
                    if(find(closedList.begin(), closedList.end(), RowNode) == closedList.end()
                        && state.grid[RowNode->Position.row][RowNode->Position.col].isWater)
                    {
                        closedList.push_back(RowNode);
                    }
                    if(find(closedList.begin(), closedList.end(), ColNode) == openList.end()
                        && state.grid[ColNode->Position.row][ColNode->Position.col].isWater)
                    {
                        closedList.push_back(ColNode);
                    }
                }
                // If it's not a water tile, we may analyse the node
                else if (!state.grid[NeighbourNode->Position.row][NeighbourNode->Position.col].isWater)
                {
                    // If the neighbour is not already analysed (not in closed or open lists)
                    if(find(closedList.begin(), closedList.end(), NeighbourNode) == closedList.end()
                        && find(openList.begin(), openList.end(), NeighbourNode) == openList.end())
                    {
                        UpdateVertex(CurrentNode, NeighbourNode, openList);
                    }
                }
                // If we can't analyse the node, we ignore it
                else
                    closedList.push_back(NeighbourNode);
            }
    }
    return path;
}