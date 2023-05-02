#include "../Public/Route.h"

Route::Route()
{
    this->start = Location();
    this->end = Location();
    this->distance = 0;
}

Route::Route(Location start, Location end, double distance)
{
    this->start = start;
    this->end = end;
    this->distance = distance;
}

std::string Route::ToString() const
{
    return "Start : " + start.ToString() + " / End : " + end.ToString() + " / Distance" + std::to_string(distance);
}
