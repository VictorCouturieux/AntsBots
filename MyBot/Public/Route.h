#pragma once

#include "Location.h"

class Route
{
public:
    Route();
    Route(Location start, Location end, double distance);

    std::string ToString();

    Location Start;
    Location End;
    double Distance;
};
