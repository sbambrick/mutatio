#include <cassert>
#include <cmath>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/location_view.h"
#include "mutatio/location_view_exchange.h"

int main() {
    // Round-trip: LLA -> ECEF -> LLA
    mutatio::LlaLocation origin{37.7749, -122.4194, 10.0};

    auto ecef = mutatio::LocationFrom<mutatio::EcefLocation>(origin);
    auto lla = mutatio::LocationFrom<mutatio::LlaLocation>(ecef);

    assert(std::abs(lla.lat - origin.lat) < 1e-9);
    assert(std::abs(lla.lon - origin.lon) < 1e-9);
    assert(std::abs(lla.alt - origin.alt) < 1e-6);

    // NED view from origin to a nearby point
    mutatio::LlaLocation point{37.7800, -122.4100, 10.0};
    auto ned = mutatio::ViewFrom<mutatio::NedLocationView>(origin, point);

    assert(ned.north > 0.0);  // point is north of origin

    return 0;
}
