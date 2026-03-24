#pragma once

#include <stdexcept>

#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"

namespace mutatio {

// EcefVelocity is frame-independent; no location is needed.
Status VelocityFrom(const EcefVelocity& in, EcefVelocity* out);

// Variant dispatch — no location needed since VelocityTypes =
// variant<EcefVelocity>.
template <class VelType, class = std::enable_if_t<
                             is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const VelocityTypes& in_vel, VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, out](const auto& in) { status = VelocityFrom(in, out); },
      },
      in_vel);
  return status;
}

// Throwing variant.
template <class OutVelType, class InVelType>
OutVelType VelocityFrom(const InVelType& in) {
  OutVelType out;
  auto status = VelocityFrom(in, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out;
}

}  // namespace mutatio
