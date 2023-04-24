#include "../Public/AStarAlgo.h"

#include <map>
#include <algorithm>

float AStarAlgo::calculateH(int row, int col, Location dest) {
    int d1 = abs(row - dest.row),
        d2 = abs(col - dest.col),
        dr = min(d1, state.rows-d1),
        dc = min(d2, state.cols-d2);
    return sqrt(dr*dr + dc*dc);
}

void AStarAlgo::setupMap()
{
    //Initialize whole map
    baseGrid.resize(state.rows, vector<Node>(state.cols, Node()));
    for (int x = 0; x < state.rows; x++) 
        for (int y = 0; y < state.cols; y++) 
            baseGrid[x][y].Position = Location(x,y);
}

void AStarAlgo::ComputeCost(Node* CurrentNode, Node* NeighbourNode)
{
    int x = abs(CurrentNode->Position.row - NeighbourNode->Position.row),
        y = abs(CurrentNode->Position.col - NeighbourNode->Position.col);
    if(x > 2)
    {
        x -= state.rows-2;
    }
    if(y > 2) 
    {
        y -= state.cols-2;
    }
    int cost = x + y;
    state.bug << "cost " << cost << endl;
    if(CurrentNode->gCost + cost < NeighbourNode->gCost)
    {
        // Set Neighbour node's parent as the current one
        NeighbourNode->Parent = CurrentNode;
        // Update Neighbour node's cost
        NeighbourNode->gCost = CurrentNode->gCost + cost;
        NeighbourNode->fCost = NeighbourNode->gCost + NeighbourNode->hCost;
        state.bug << " - " << NeighbourNode->ToString() << endl;
    }
}

void AStarAlgo::UpdateVertex(Node* CurrentNode, Node* NeighbourNode, vector<Node*> &openList)
{
    float oldCost = NeighbourNode->gCost;
    state.bug << "->" << NeighbourNode->Position.ToString();
    ComputeCost(CurrentNode, NeighbourNode);
    if(NeighbourNode->gCost < oldCost)
    {
        const auto it = find(openList.begin(), openList.end(), NeighbourNode);
        if(it != openList.end())
            openList.erase(it);  
        openList.push_back(NeighbourNode);
    }       
}

vector<Location> AStarAlgo::aStar(Location antLoc, Location destLoc) {

    vector<Location> path;
    // Réduire taille map : carré antLoc->dest

    vector<vector<Node>> grid = baseGrid;

    // Calculate h(x) with destLoc
    for (int x = 0; x < state.rows; x++) 
        for (int y = 0; y < state.cols; y++) 
            grid[x][y].hCost = calculateH(x,y, destLoc);
    
    // Init both open (nodes to be considered) and closed (nodes to ignore) lists
    vector<Node*> closedList;
    vector<Node*> openList;

    // Start position initialisation
    grid[antLoc.row][antLoc.col].gCost = 0;
    grid[antLoc.row][antLoc.col].fCost = grid[antLoc.row][antLoc.col].hCost;
    grid[antLoc.row][antLoc.col].Parent = &grid[antLoc.row][antLoc.col];

    state.bug << "From " << antLoc.ToString() << " To " << destLoc.ToString() << endl;
    openList.push_back(&grid[antLoc.row][antLoc.col]);
    while(!openList.empty())
    {
        // Sort the array in a way to have the lowest fCost in last position
        sort( openList.begin(), openList.end(), [](Node* a, Node* b) { return a->fCost > b->fCost; } );
        // Pop the last Node in openList
        Node* CurrentNode = openList.back();
        openList.pop_back();

        state.bug << CurrentNode->Position.ToString() << "<-" << CurrentNode->Parent->Position.ToString() << endl;
        // If we found the destination
        if(CurrentNode->Position == destLoc)
        {
            state.bug << "Found path!" << endl;
            // Build the path by seeking for startLocation from parents
            path.push_back(CurrentNode->Position);
            while(CurrentNode->Position != antLoc)
            {
                CurrentNode = CurrentNode->Parent;
                path.push_back(Location(CurrentNode->Position));
            }
            std::reverse(path.begin(),path.end());
            for(Location loc : path)
                state.bug << loc.ToString() << "->";
            state.bug << endl;
            break;
        }

        // Otherwise, we analyse its neighbours
        closedList.push_back(CurrentNode);
        for(int x=-1; x <= 1; x++)
            for(int y=-1; y <= 1; y++)
            {
                // TODO : Check for borders neighbours
                int posX = CurrentNode->Position.row + x;
                int posY = CurrentNode->Position.col + y;
                if(posX >= state.rows) posX = 0;
                else if(posX < 0) posX = state.rows-1;
                if(posY >= state.cols) posY = 0;
                else if(posY < 0) posY = state.cols-1;
                
                Node* NeighbourNode = &grid[posX][posY];
                if(!NeighbourNode)
                    state.bug << "Node inexistant" << endl;
                if(state.isFree(NeighbourNode->Position))
                {
                    state.bug << "Node free : " << NeighbourNode->Position.ToString() << endl;
                    // If the neighbour is not already analysed (not in closed or open lists)
                    if(find(closedList.begin(), closedList.end(), NeighbourNode) == closedList.end()
                        && find(openList.begin(), openList.end(), NeighbourNode) == openList.end())
                    {
                        UpdateVertex(CurrentNode, NeighbourNode, openList);
                    }
                } else
                {
                    state.bug << "Node unreachable" << endl;
                    closedList.push_back(NeighbourNode);
                }
            }
    }

    state.bug << "Pathfinding ended" << endl;
    return path;
}