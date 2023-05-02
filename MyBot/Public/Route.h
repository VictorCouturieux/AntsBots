#pragma once

#include "Location.h"

/**
 * \brief Ant Route containing information about Start and End locations, and its distance
 */
class Route
{
public:
    Route();
    Route(Location start, Location end, double distance);

    /**
     * \brief Convert the Route to a string, for debug purposes
     */
    std::string ToString();

    Location Start;
    Location End;
    double Distance;
};
