#pragma once

#include <stdexcept>

#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"

namespace mutatio {

Status VelocityFrom(const EcefVelocity& in, EcefVelocity* out);

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

template <class OutVelType, class InVelType>
OutVelType VelocityFrom(const InVelType& in) {
  OutVelType out;
  auto status = VelocityFrom(in, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityFrom.");
  }

  return out;
}

}  // namespace mutatio
