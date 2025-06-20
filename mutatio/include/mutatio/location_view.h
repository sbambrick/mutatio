#pragma once

#include <variant>

#include "mutatio/units.h"

namespace mutatio {

struct AerLocationView {
  Degree azimuth;
  Degree elevation;
  Meter range;
};

struct EcefLocationView {
  Meter x;
  Meter y;
  Meter z;
};

struct NedLocationView {
  Meter north;
  Meter east;
  Meter down;
};

using LocationViewTypes =
    std::variant<AerLocationView, EcefLocationView, NedLocationView>;

}  // namespace mutatio
