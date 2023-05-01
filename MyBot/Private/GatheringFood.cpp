#include "../Public/GatheringFood.h"
#include "../Public/Bot.h"

void GatheringFood::makeMoves()
{
    Behaviour::makeMoves();
    
    /////       ***** Food gathering *****      /////
    vector<Route> foodRoutes ;
    for(Location food : bot->state.food)
        for(Location ant : bot->state.myAnts)
        {
            const double distance = bot->state.EuclideanDistance(ant, food);
            if(distance < 20)
                foodRoutes.emplace_back(ant, food, distance);
        }
    // Sort the foodRoutes list in a way that we have the shortests distances first
    sort( foodRoutes.begin(), foodRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );

    for(Route food : foodRoutes)
        if(!bot->targets.containsKey(food.End) && !bot->targets.containsValue(food.Start))
            doMoveLocation(food.Start, food.End, true);

    /////       ***** Attacking ennemies *****      /////
    // attack hills
    /// Attack hills
    vector<Route> hillRoutes = getShortestRoutesTo(bot->state.enemyHills, bot->state.viewradius * 2);
    for (Route route : hillRoutes)
    {
        doMoveLocation(route.Start, route.End, true);
    }
    
    /////       ***** Exploration *****      /////
    // explore unseen areas
    Exploration(bot->state.viewradius * 2.5f);
    // Moving out from our hills
    moveOutFromHills();

    // Ants without any orders : farmers ? 
    for(Location antLoc : bot->state.myAnts)
    {
        // If the ant doesn't have any route assigned
        if (!bot->orders.containsValue(antLoc))
        {
            bot->state.bug << antLoc.ToString() << " doesn't have any order..." << endl;
            int randX = rand() % bot->state.rows;
            int randY = rand() % bot->state.cols;
            while(bot->state.grid[randX][randY].isWater)
            {
                randX = rand() % bot->state.rows;
                randY = rand() % bot->state.cols;
            }
            doMoveLocation(antLoc, Location(randX, randY), false);
            
            /*
            vector<Route> farmRoutes ;
            // search the closest farm Location with the ant
            for (Location farmLoc : farmLocs->farmingList)
            {
                //if the farm loc is visible by ants group
                if(bot->state.grid[farmLoc.row][farmLoc.col].isVisible)//distance < bot->state.viewradius)
                {
                    const double distance = bot->state.EuclideanDistance(antLoc, farmLoc);
                    farmRoutes.emplace_back(antLoc, farmLoc, distance);
                }
            }

            //if the closest list of farm location is not empty
            if (!farmRoutes.empty())
            {
                //sort the farm location the ant found
                sort( farmRoutes.begin(), farmRoutes.end(), [](Route a, Route b) { return a.Distance < b.Distance; } );
                // for each route to go on farm location, we found the best way for this ant
                for(Route farmR : farmRoutes)
                {
                    //if the farm location is free 
                    if (!bot->targets.containsKey(farmR.End) && !bot->orders.containsKey(farmR.End) && !bot->state.grid[farmR.End.row][farmR.End.col].ant)
                    {
                        bot->state.bug << farmR.ToString() << endl;
                        Location homeFarm = farmR.End;
                        //if the homeFarm ant found is in the water, search the closest location on the ground
                        if (!bot->state.grid[homeFarm.row][homeFarm.col].isWater || farmLocs->findClosestLoc(homeFarm))
                        {
                            doMoveLocation(farmR.Start, homeFarm, true);
                            break;
                        }
                    }
                }
            }
            */
        }
    }
}


void GatheringFood::Init()
{
    Behaviour::Init();
    farmLocs = new FarmingLocation(bot->state);
}

