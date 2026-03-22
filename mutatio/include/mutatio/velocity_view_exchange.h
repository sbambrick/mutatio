#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

// EcefVelocityView — no location required.
Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        EcefVelocityView* out);
Status VelocityFrom(const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out);

// NedVelocityView — a location is required to define the NED frame.
Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        const LlaLocation& loc, NedVelocityView* out);
Status VelocityViewFrom(const NedVelocity& origin, const NedVelocity& point,
                        const LlaLocation& loc, NedVelocityView* out);

Status VelocityFrom(const EcefVelocity& origin, const NedVelocityView& view,
                    const LlaLocation& loc, EcefVelocity* out);
Status VelocityFrom(const NedVelocity& origin, const NedVelocityView& view,
                    const LlaLocation& loc, NedVelocity* out);

// LocType overloads — convert any location type to LlaLocation, then call the
// specific overload above.
template <class LocType>
Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        const LocType& loc, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin, point, lla, out);
}

template <class LocType>
Status VelocityViewFrom(const NedVelocity& origin, const NedVelocity& point,
                        const LocType& loc, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin, point, lla, out);
}

template <class LocType>
Status VelocityFrom(const EcefVelocity& origin, const NedVelocityView& view,
                    const LocType& loc, EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin, view, lla, out);
}

template <class LocType>
Status VelocityFrom(const NedVelocity& origin, const NedVelocityView& view,
                    const LocType& loc, NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin, view, lla, out);
}

// Throwing variants — no location (EcefVelocityView only).
template <class OutViewType, class OriginType, class PointType>
OutViewType VelocityViewFrom(const OriginType& origin, const PointType& point) {
  OutViewType out;
  auto status = VelocityViewFrom(origin, point, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityViewFrom.");
  }

  return out;
}

template <class OutVelType, class OriginType, class ViewType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, ViewType>>>
OutVelType VelocityFrom(const OriginType& origin, const ViewType& view) {
  OutVelType out;
  auto status = VelocityFrom(origin, view, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityFrom.");
  }

  return out;
}

// Throwing variants — with location (required for NedVelocityView).
template <class OutViewType, class OriginType, class PointType, class LocType>
OutViewType VelocityViewFrom(const OriginType& origin, const PointType& point,
                             const LocType& loc) {
  OutViewType out;
  auto status = VelocityViewFrom(origin, point, loc, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityViewFrom.");
  }

  return out;
}

template <class OutVelType, class OriginType, class ViewType, class LocType>
OutVelType VelocityFrom(const OriginType& origin, const ViewType& view,
                        const LocType& loc) {
  OutVelType out;
  auto status = VelocityFrom(origin, view, loc, &out);

  switch (status) {
    case Status::ERROR:
      throw std::invalid_argument("ERROR in VelocityFrom.");
  }

  return out;
}

// Variant dispatch — no location. EcefVelocityView is dispatched normally;
// NedVelocityView requires a location and returns Status::ERROR here.
template <class ViewType, class = std::enable_if_t<
                              is_one_of_variants_types<VelocityViewTypes, ViewType>>>
Status VelocityViewFrom(const VelocityTypes& origin, const VelocityTypes& point,
                        ViewType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &point, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, out](const auto& pt) {
                      if constexpr (std::is_same_v<ViewType, EcefVelocityView>) {
                        status = VelocityViewFrom(orig, pt, out);
                      } else {
                        status = Status::ERROR;
                      }
                    },
                },
                point);
          },
      },
      origin);
  return status;
}

template <class VelType, class = std::enable_if_t<
                             is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const VelocityTypes& origin,
                    const VelocityViewTypes& view_variant, VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, out](const auto& view) {
                      if constexpr (std::is_same_v<std::decay_t<decltype(view)>,
                                                   EcefVelocityView>) {
                        status = VelocityFrom(orig, view, out);
                      } else {
                        status = Status::ERROR;
                      }
                    },
                },
                view_variant);
          },
      },
      origin);
  return status;
}

// Variant dispatch — with location. For NedVelocityView, same-type
// origin/output pairs are dispatched; cross-type returns Status::ERROR.
// For EcefVelocityView the location is ignored.
template <class ViewType, class LocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, ViewType>>>
Status VelocityViewFrom(const VelocityTypes& origin, const VelocityTypes& point,
                        const LocType& loc, ViewType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &point, &loc, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, &loc, out](const auto& pt) {
                      if constexpr (std::is_same_v<ViewType, EcefVelocityView>) {
                        status = VelocityViewFrom(orig, pt, out);
                      } else if constexpr (
                          std::is_same_v<std::decay_t<decltype(orig)>,
                                         std::decay_t<decltype(pt)>>) {
                        status = VelocityViewFrom(orig, pt, loc, out);
                      } else {
                        status = Status::ERROR;
                      }
                    },
                },
                point);
          },
      },
      origin);
  return status;
}

template <class VelType, class LocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const VelocityTypes& origin,
                    const VelocityViewTypes& view_variant, const LocType& loc,
                    VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, &loc, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, &loc, out](const auto& view) {
                      if constexpr (std::is_same_v<std::decay_t<decltype(view)>,
                                                   EcefVelocityView>) {
                        status = VelocityFrom(orig, view, out);
                      } else if constexpr (
                          std::is_same_v<std::decay_t<decltype(orig)>, VelType>) {
                        status = VelocityFrom(orig, view, loc, out);
                      } else {
                        status = Status::ERROR;
                      }
                    },
                },
                view_variant);
          },
      },
      origin);
  return status;
}

}  // namespace mutatio
