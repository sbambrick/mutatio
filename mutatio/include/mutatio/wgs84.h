#pragma once

#include <cmath>

namespace mutatio {

// WGS84 ellipsoid parameters.
constexpr double kWgs84A = 6378137.0;            // Semi-major axis (m)
constexpr double kWgs84F = 1.0 / 298.257223563;  // Flattening

// Degrees-to-radians conversion factor.
constexpr double kDegToRad = M_PI / 180.0;

}  // namespace mutatio
