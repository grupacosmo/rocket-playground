#pragma once
#include <WString.h>

namespace gps
{
    struct Location
    {
        double latitude;
        double longitude;
        String time;
    };

    void begin();

    void fetchLocation();

    Location parseLocation();

    void printLocation(Location location);

}