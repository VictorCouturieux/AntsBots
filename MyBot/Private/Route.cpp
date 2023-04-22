#include "../Public/Route.h"

Route::Route()
{
    this->Start = Location();
    this->End = Location();
    this->Distance = 0;
}

Route::Route(Location start, Location end, double distance)
{
    this->Start = start;
    this->End = end;
    this->Distance = distance;
}

std::string Route::ToString()
{
    return "Start : " + Start.ToString() + " / End : " + End.ToString() + " / Distance" + std::to_string(Distance);
}
