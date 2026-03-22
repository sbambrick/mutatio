#pragma once

#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct EcefVelocityView {
  MetersPerSecond vx;
  MetersPerSecond vy;
  MetersPerSecond vz;
};

using VelocityViewTypes = std::variant<EcefVelocityView>;

}  // namespace mutatio
