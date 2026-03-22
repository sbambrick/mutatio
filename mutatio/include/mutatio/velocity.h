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

using VelocityTypes = std::variant<EcefVelocity, NedVelocity>;

}  // namespace mutatio
