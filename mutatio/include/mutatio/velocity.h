#pragma once

#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct EcefVelocity {
  MetersPerSecond vx;
  MetersPerSecond vy;
  MetersPerSecond vz;
};

using VelocityTypes = std::variant<EcefVelocity>;

}  // namespace mutatio
