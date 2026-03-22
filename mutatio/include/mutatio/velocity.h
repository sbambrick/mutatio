#pragma once

#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct EcefVelocity {
  MetersPerSecond vx;
  MetersPerSecond vy;
  MetersPerSecond vz;
};

struct NedVelocity {
  MetersPerSecond vnorth;
  MetersPerSecond veast;
  MetersPerSecond vdown;
};

struct AerVelocity {
  DegreesPerSecond vazimuth;
  DegreesPerSecond velevation;
  MetersPerSecond  vrange;
};

using VelocityTypes = std::variant<EcefVelocity, NedVelocity, AerVelocity>;

}  // namespace mutatio
