#pragma once

#include <cmath>
#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct LlaLocation {
  Degree lat;
  Degree lon;
  Meter alt;
};

struct EcefLocation {
  Meter x;
  Meter y;
  Meter z;
};

using LocationTypes = std::variant<LlaLocation, EcefLocation>;

}  // namespace mutatio
