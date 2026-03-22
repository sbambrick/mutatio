#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"

namespace mutatio {

Status VelocityFrom(const EcefVelocity& in, const LlaLocation& loc,
                    NedVelocity* out);
Status VelocityFrom(const NedVelocity& in, const LlaLocation& loc,
                    EcefVelocity* out);

// LocType overloads convert the location to LlaLocation, then call the
// specific overload above. This allows any LocationTypes value to be passed.
template <class LocType>
Status VelocityFrom(const EcefVelocity& in, const LocType& loc,
                    NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(in, lla, out);
}

template <class LocType>
Status VelocityFrom(const NedVelocity& in, const LocType& loc,
                    EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(in, lla, out);
}

// Variant dispatch — dispatches over VelocityTypes with a location. Same-type
// conversions (e.g. ECEF->ECEF) are not supported and return Status::ERROR.
template <class VelType, class LocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const VelocityTypes& in_vel, const LocType& loc,
                    VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &loc, out](const auto& in) {
            if constexpr (!std::is_same_v<std::decay_t<decltype(in)>, VelType>) {
              status = VelocityFrom(in, loc, out);
            } else {
              status = Status::ERROR;
            }
          },
      },
      in_vel);
  return status;
}

// Throwing variant — frame conversions requiring a location.
template <class OutVelType, class InVelType, class LocType>
OutVelType VelocityFrom(const InVelType& in, const LocType& loc) {
  OutVelType out;
  auto status = VelocityFrom(in, loc, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityFrom.");
  }

  return out;
}

}  // namespace mutatio
