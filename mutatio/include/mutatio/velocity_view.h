#pragma once

#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct EcefVelocityView {
  MetersPerSecond vx;
  MetersPerSecond vy;
  MetersPerSecond vz;
};

struct NedVelocityView {
  MetersPerSecond vnorth;
  MetersPerSecond veast;
  MetersPerSecond vdown;
};

struct AerVelocityView {
  DegreesPerSecond vazimuth;
  DegreesPerSecond velevation;
  MetersPerSecond vrange;
};

using VelocityViewTypes =
    std::variant<EcefVelocityView, NedVelocityView, AerVelocityView>;

}  // namespace mutatio
