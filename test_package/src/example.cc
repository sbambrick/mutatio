#include <cassert>
#include <cmath>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"

int main() {
    // Round-trip: LLA -> ECEF -> LLA
    mutatio::LlaLocation origin{37.7749, -122.4194, 10.0};

    auto ecef = mutatio::LocationFrom<mutatio::EcefLocation>(origin);
    auto lla = mutatio::LocationFrom<mutatio::LlaLocation>(ecef);

    assert(std::abs(lla.lat - origin.lat) < 1e-9);
    assert(std::abs(lla.lon - origin.lon) < 1e-9);
    assert(std::abs(lla.alt - origin.alt) < 1e-6);

    return 0;
}
