#include "../Public/AStarAlgo.h"

#include <map>
#include <algorithm>

void AStarAlgo::SetupMap()
{
    _baseGrid.resize(_state.rows, vector<Node>(_state.cols, Node()));
    for (int x = 0; x < _state.rows; x++) 
        for (int y = 0; y < _state.cols; y++) 
            _baseGrid[x][y].position = Location(x,y);
}

void AStarAlgo::computeCost(Node* CurrentNode, Node* NeighbourNode) const
{
    // Manhattan distance between the neighbour
    int x = abs(CurrentNode->position.row - NeighbourNode->position.row),
        y = abs(CurrentNode->position.col - NeighbourNode->position.col);
    // Borders
    if(x > 2) x -= _state.rows-2;
    if(y > 2) y -= _state.cols-2;
    
    int cost = x + y;
    // If this path will be shorter to access the neighbour node, we update its costs
    if(CurrentNode->gCost + cost < NeighbourNode->gCost)
    {
        // Set Neighbour node's parent as the current one
        NeighbourNode->parent = CurrentNode;
        // Update Neighbour node's cost
        NeighbourNode->gCost = CurrentNode->gCost + cost;
        NeighbourNode->fCost = NeighbourNode->gCost + NeighbourNode->hCost;
    }
}

void AStarAlgo::updateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList) const
{
    const double oldCost = NeighbourNode->gCost;
    computeCost(CurrentNode, NeighbourNode);
    
    // If passing by the CurrentNode gives a shorter path to the NeighbourNode, we add it to the openList
    if(NeighbourNode->gCost < oldCost)
    {
        const auto it = find(openList.begin(), openList.end(), NeighbourNode);
        if(it != openList.end()) openList.erase(it);  
        openList.push_back(NeighbourNode);
    }       
}

vector<Location> AStarAlgo::GetPath(Location antLoc, Location destLoc) {
    vector<Location> path;

    vector<vector<Node>> grid = _baseGrid;

    // Calculate h(x) with destLoc
    for (int x = 0; x < _state.rows; x++) 
        for (int y = 0; y < _state.cols; y++) 
            grid[x][y].hCost = _state.ManhattanDistance(Location(x,y), destLoc); // TODO : check si c'est pas mieux euclide
    
    // Init both open (nodes to be considered) and closed (nodes to ignore) lists
    vector<Node*> closedList;
    vector<Node*> openList;

    // Start position initialisation
    grid[antLoc.row][antLoc.col].gCost = 0;
    grid[antLoc.row][antLoc.col].fCost = grid[antLoc.row][antLoc.col].hCost;
    grid[antLoc.row][antLoc.col].parent = &grid[antLoc.row][antLoc.col];
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
        if(CurrentNode->position == destLoc)
        {
            // Build the path by seeking for Start Location from parents
            path.push_back(CurrentNode->position);
            while(CurrentNode->position != antLoc)
            {
                CurrentNode = CurrentNode->parent;
                Location CurrentLocation = Location(CurrentNode->position);

                // If the Current Location has two directions from the last step, we need to transform it as a Manhattan movement
                array< int, 2 > directions;
                if (!path.empty() && _state.GetClosestDirections(path[path.size()-1], CurrentLocation, directions) == 2)
                {
                    Location loc = _state.GetLocation(antLoc, directions[0]);
                    if (!_state.grid[loc.row][loc.col].isWater)
                    {
                        path.push_back(_state.GetLocation(path[path.size()-1], directions[0]));
                        path.push_back(_state.GetLocation(path[path.size()-1], directions[1]));
                    } else
                    {
                        path.push_back(_state.GetLocation(path[path.size()-1], directions[1]));
                        path.push_back(_state.GetLocation(path[path.size()-1], directions[0]));
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
                int posX = CurrentNode->position.row + x;
                int posY = CurrentNode->position.col + y;
                // Check for Neighbours that are outside of map borders
                if(posX >= _state.rows) posX = 0;
                else if(posX < 0) posX = _state.rows-1;
                if(posY >= _state.cols) posY = 0;
                else if(posY < 0) posY = _state.cols-1;
                
                Node* NeighbourNode = &grid[posX][posY];

                // Firstly, if we are analysing a diagonal, we have to check if both of col and row nodes are water tiles
                if (abs(x) + abs(y) == 2)
                {
                    Node* RowNode = &grid[CurrentNode->position.row][posY];
                    Node* ColNode = &grid[posX][CurrentNode->position.col];
                    if(find(closedList.begin(), closedList.end(), RowNode) == closedList.end()
                        && _state.grid[RowNode->position.row][RowNode->position.col].isWater)
                    {
                        closedList.push_back(RowNode);
                    }
                    if(find(closedList.begin(), closedList.end(), ColNode) == openList.end()
                        && _state.grid[ColNode->position.row][ColNode->position.col].isWater)
                    {
                        closedList.push_back(ColNode);
                    }
                }
                // If it's not a water tile, we may analyse the node
                else if (!_state.grid[NeighbourNode->position.row][NeighbourNode->position.col].isWater)
                {
                    // If the neighbour is not already analysed (not in closed or open lists)
                    if(find(closedList.begin(), closedList.end(), NeighbourNode) == closedList.end()
                        && find(openList.begin(), openList.end(), NeighbourNode) == openList.end())
                    {
                        updateVertex(CurrentNode, NeighbourNode, openList);
                    }
                }
                // If we can't analyse the node, we ignore it
                else
                    closedList.push_back(NeighbourNode);
            }
    }
    return path;
}