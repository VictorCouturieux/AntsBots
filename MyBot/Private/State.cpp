#include "../Public/State.h"

using namespace std;

//constructor
State::State()
{
    gameover = 0;
    turn = 0;
    bug.open("./debug.txt");
};

//deconstructor
State::~State()
{
    bug.close();
};

//sets the state up
void State::setup()
{
    grid = vector<vector<Square> >(rows, vector<Square>(cols, Square()));
};

//resets all non-water squares to land and clears the bots ant vector
void State::reset()
{
    myAnts.clear();
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    food.clear();
    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!grid[row][col].isWater)
                grid[row][col].reset();
};

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction)
{
    cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << endl;

    Location nLoc = getLocation(loc, direction);
    grid[nLoc.row][nLoc.col].ant = grid[loc.row][loc.col].ant;
    grid[loc.row][loc.col].ant = -1;
};

//returns the euclidean distance between two locations with the edges wrapped
double State::EuclideanDistance(const Location &loc1, const Location &loc2)
{
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, rows-d1),
        dc = min(d2, cols-d2);
    return sqrt(dr*dr + dc*dc);
};

//returns the euclidean distance between two locations with the edges wrapped
double State::ManhattanDistance(const Location &loc1, const Location &loc2)
{
    float d = abs(loc1.row - loc2.row) + abs(loc1.col - loc2.col);
    return d;
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction)
{
    return Location( (loc.row + DIRECTIONS[direction][0] + rows) % rows,
                     (loc.col + DIRECTIONS[direction][1] + cols) % cols );
};

bool State::directAccessTarget(const Location &loc1, const Location &loc2)
{
    Location processLoc = loc1;
    array< int, 2 > directions;
    while (processLoc != loc2)
    {
        int nbDir = getClosestDirections(processLoc, loc2, directions);
        Location searchLoc;
        for (int i = 0; i < nbDir; ++i)
        {
            searchLoc = getLocation(processLoc, directions[i]);
            if (grid[searchLoc.row][searchLoc.col].isWater)
            {
                if (nbDir == 2 && i == 0)
                {
                    searchLoc = getLocation(processLoc, directions[abs(i - 1)]);
                    if (nbDir == 2 && grid[searchLoc.row][searchLoc.col].isWater) return false;
                    searchLoc = getLocation(processLoc, directions[i]);
                    if (nbDir == 2 && grid[searchLoc.row][searchLoc.col].isWater) return false;
                }
                else
                    return false;
            }
            processLoc = searchLoc;
        }
    }
    return true;
}

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/

void State::updateVisionInformation()
{
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a=0; a<(int) myAnts.size(); a++)
    {
        sLoc = myAnts[a];
        locQueue.push(sLoc);

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        grid[sLoc.row][sLoc.col].isVisible = 1;
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && EuclideanDistance(sLoc, nLoc) <= viewradius)
                {
                    grid[nLoc.row][nLoc.col].isVisible = 1;
                    locQueue.push(nLoc);
                }
                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
}

bool State::isFree(const Location& loc)
{
    for (int antIdx = 0 ; antIdx < myAnts.size() ; ++antIdx )
        if ( grid[loc.row][loc.col].isWater || loc == myAnts[ antIdx ] ) 
            return false;

    return true;
}

int State::getClosestDirections(const Location& antLoc, const Location& destLoc, std::array<int, 2>& directions)
{
    int nbDirections=0;

    if (antLoc.row > destLoc.row)
        if (abs(antLoc.row - destLoc.row) > abs(destLoc.row + rows - antLoc.row))
            directions[ nbDirections++ ] = 2; // S
        else
            directions[ nbDirections++ ] = 0; // N
    else if (antLoc.row < destLoc.row)
        if (abs(destLoc.row - antLoc.row) > abs(antLoc.row + rows - destLoc.row))
            directions[ nbDirections++ ] = 0; // N
        else
            directions[ nbDirections++ ] = 2; // S
            
    if(antLoc.col > destLoc.col)
        //directions[ nbDirections++ ] = 3; // W
        if (abs(antLoc.col - destLoc.col) > abs(destLoc.col + cols - antLoc.col))
            directions[ nbDirections++ ] = 1; // E
        else
            directions[ nbDirections++ ] = 3; // W
            
    else if(antLoc.col < destLoc.col)
        //directions[ nbDirections++ ] = 1; // E
        if (abs(destLoc.col - antLoc.col) > abs(antLoc.col + cols - destLoc.col))
            directions[ nbDirections++ ] = 3; // W
        else
            directions[ nbDirections++ ] = 1; // E
    
    return nbDirections;
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const State &state)
{
    for(int row=0; row<state.rows; row++)
    {
        for(int col=0; col<state.cols; col++)
        {
            if(state.grid[row][col].isWater)
                os << '%';
            else if(state.grid[row][col].isFood)
                os << '*';
            else if(state.grid[row][col].isHill)
                os << (char)('A' + state.grid[row][col].hillPlayer);
            else if(state.grid[row][col].ant >= 0)
                os << (char)('a' + state.grid[row][col].ant);
            else if(state.grid[row][col].isVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

//input function
istream& operator>>(istream &is, State &state)
{
    int row, col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state.gameover = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> state.turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(state.turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> state.loadtime;
            else if(inputType == "turntime")
                is >> state.turntime;
            else if(inputType == "rows")
                is >> state.rows;
            else if(inputType == "cols")
                is >> state.cols;
            else if(inputType == "turns")
                is >> state.turns;
            else if(inputType == "player_seed")
                is >> state.seed;
            else if(inputType == "viewradius2")
            {
                is >> state.viewradius;
                state.viewradius = sqrt(state.viewradius);
            }
            else if(inputType == "attackradius2")
            {
                is >> state.attackradius;
                state.attackradius = sqrt(state.attackradius);
            }
            else if(inputType == "spawnradius2")
            {
                is >> state.spawnradius;
                state.spawnradius = sqrt(state.spawnradius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                state.timer.start();
                break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else
    {
        //reads information about the current turn
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.grid[row][col].isWater = 1;
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;
                state.grid[row][col].isFood = 1;
                state.food.push_back(Location(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                if(player == 0)
                    state.myAnts.push_back(Location(row, col));
                else
                    state.enemyAnts.push_back(Location(row, col));
            }
            else if(inputType == "d") //dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if(player == 0)
                    state.myHills.push_back(Location(row, col));
                else
                    state.enemyHills.push_back(Location(row, col));

            }
            else if(inputType == "players") //player information
                is >> state.noPlayers;
            else if(inputType == "scores") //score information
            {
                state.scores = vector<double>(state.noPlayers, 0.0);
                for(int p=0; p<state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state.gameover)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }
    }

    return is;
};
