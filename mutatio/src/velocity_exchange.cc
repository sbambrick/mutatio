#include "mutatio/velocity_exchange.h"

#include <cmath>

#include "GeographicLib/Constants.hpp"

namespace mutatio {

// Rotates an ECEF velocity into the NED frame at the given location using the
// standard ECEF-to-NED rotation matrix:
//   [ -sin(lat)cos(lon)  -sin(lat)sin(lon)   cos(lat) ]
//   [ -sin(lon)           cos(lon)            0        ]
//   [ -cos(lat)cos(lon)  -cos(lat)sin(lon)  -sin(lat)  ]
Status VelocityFrom(const EcefVelocity& in, const LlaLocation& loc,
                    NedVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);

  *out = NedVelocity{
      -sin_lat * cos_lon * in.vx - sin_lat * sin_lon * in.vy + cos_lat * in.vz,
      -sin_lon * in.vx + cos_lon * in.vy,
      -cos_lat * cos_lon * in.vx - cos_lat * sin_lon * in.vy - sin_lat * in.vz,
  };
  return Status::SUCCESS;
}

// Rotates a NED velocity back into the ECEF frame using the transpose of the
// ECEF-to-NED rotation matrix.
Status VelocityFrom(const NedVelocity& in, const LlaLocation& loc,
                    EcefVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);

  *out = EcefVelocity{
      -sin_lat * cos_lon * in.vnorth - sin_lon * in.veast -
          cos_lat * cos_lon * in.vdown,
      -sin_lat * sin_lon * in.vnorth + cos_lon * in.veast -
          cos_lat * sin_lon * in.vdown,
      cos_lat * in.vnorth - sin_lat * in.vdown,
  };
  return Status::SUCCESS;
}

}  // namespace mutatio
