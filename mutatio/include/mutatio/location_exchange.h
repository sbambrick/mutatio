#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"

namespace mutatio {

Status LocationFrom(const LlaLocation &in, LlaLocation *out);
Status LocationFrom(const LlaLocation &in, EcefLocation *out);
Status LocationFrom(const EcefLocation &in, LlaLocation *out);
Status LocationFrom(const EcefLocation &in, EcefLocation *out);

template <class LocType, class = std::enable_if_t<
                             is_one_of_variants_types<LocationTypes, LocType>>>
Status LocationFrom(const LocationTypes &in_loc, LocType *out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, out](const auto &in) { status = LocationFrom(in, out); },
      },
      in_loc);
  return status;
}

template <class OutLocType, class InLocType>
OutLocType LocationFrom(const InLocType &in) {
  OutLocType out;
  auto status = LocationFrom(in, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in LocationFrom.");
  }

  return out;
}

}  // namespace mutatio
