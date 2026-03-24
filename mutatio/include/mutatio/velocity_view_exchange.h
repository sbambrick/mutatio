#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/location_view.h"
#include "mutatio/location_view_exchange.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

// Two-location VelocityViewFrom — origin_loc and point_loc define the geometry.
// EcefVelocityView: locations accepted for API consistency but unused.
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        EcefVelocityView* out_vel);

// NedVelocityView: origin_loc defines the NED frame; point_loc unused.
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        NedVelocityView* out_vel);

// AerVelocityView: both locations define the observer-target geometry.
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        AerVelocityView* out_vel);

// Two-location VelocityFrom — reconstruct EcefVelocity from origin + view.
// EcefVelocityView: both locations unused.
// NedVelocityView: origin_loc defines the NED frame; point_loc unused.
// AerVelocityView: both locations define the observer-target geometry.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin_vel,
                    const EcefVelocityView& view_vel, EcefVelocity* out_vel);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin_vel,
                    const NedVelocityView& view_vel, EcefVelocity* out_vel);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin_vel,
                    const AerVelocityView& view_vel, EcefVelocity* out_vel);

// Two-location LocType overloads — convert any location type to LlaLocation,
// then delegate. PointLocType is constrained to non-LocationView types to avoid
// ambiguity with the LocationView overloads below.
template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        EcefVelocityView* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin_vel, point_vel,
                          out_vel);
}

template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        NedVelocityView* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin_vel, point_vel,
                          out_vel);
}

template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        AerVelocityView* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin_vel, point_vel,
                          out_vel);
}

template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc,
                    const EcefVelocity& origin_vel,
                    const EcefVelocityView& view_vel, EcefVelocity* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin_vel, view_vel, out_vel);
}

template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc,
                    const EcefVelocity& origin_vel,
                    const NedVelocityView& view_vel, EcefVelocity* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin_vel, view_vel, out_vel);
}

template <class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              !is_one_of_variants_types<LocationViewTypes, PointLocType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc,
                    const EcefVelocity& origin_vel,
                    const AerVelocityView& view_vel, EcefVelocity* out_vel) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin_vel, view_vel, out_vel);
}

// LocationView overloads — origin_loc + a LocationView encoding the relative
// position of the point. Delegates to the two-location form after
// reconstructing point_lla via LocationFrom(origin_loc, point_loc, &point_lla).
template <class LocViewType, class VelViewOutType,
          class = std::enable_if_t<
              is_one_of_variants_types<LocationViewTypes, LocViewType>>,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, VelViewOutType>>>
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LocViewType& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        VelViewOutType* out_vel) {
  LlaLocation point_lla;
  auto stat = LocationFrom(origin_loc, point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_loc, point_lla, origin_vel, point_vel,
                          out_vel);
}

template <class LocViewType, class VelViewType,
          class = std::enable_if_t<
              is_one_of_variants_types<LocationViewTypes, LocViewType>>,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, VelViewType>>>
Status VelocityFrom(const LlaLocation& origin_loc, const LocViewType& point_loc,
                    const EcefVelocity& origin_vel, const VelViewType& view_vel,
                    EcefVelocity* out_vel) {
  LlaLocation point_lla;
  auto stat = LocationFrom(origin_loc, point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_loc, point_lla, origin_vel, view_vel, out_vel);
}

// OriginLocType + LocationView overloads — convert origin to LlaLocation, then
// delegate to the LlaLocation + LocationView form.
template <class OriginLocType, class LocViewType, class VelViewOutType,
          class = std::enable_if_t<
              is_one_of_variants_types<LocationViewTypes, LocViewType>>,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, VelViewOutType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const LocViewType& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        VelViewOutType* out_vel) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_loc, origin_vel, point_vel,
                          out_vel);
}

template <class OriginLocType, class LocViewType, class VelViewType,
          class = std::enable_if_t<
              is_one_of_variants_types<LocationViewTypes, LocViewType>>,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, VelViewType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const LocViewType& point_loc,
                    const EcefVelocity& origin_vel, const VelViewType& view_vel,
                    EcefVelocity* out_vel) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_loc, origin_vel, view_vel, out_vel);
}

// Throwing wrapper — two locations (also covers LocationView as second arg).
template <class OutVelType, class OriginVelType, class ViewVelType,
          class OriginLocType, class PointLocType>
OutVelType VelocityFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const OriginVelType& origin_vel,
                        const ViewVelType& view_vel) {
  OutVelType out_vel;
  auto status =
      VelocityFrom(origin_loc, point_loc, origin_vel, view_vel, &out_vel);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out_vel;
}

// Throwing variant — two locations (also covers LocationView as second arg).
template <class OutViewVelType, class OriginVelType, class PointVelType,
          class OriginLocType, class PointLocType>
OutViewVelType VelocityViewFrom(const OriginLocType& origin_loc,
                                const PointLocType& point_loc,
                                const OriginVelType& origin_vel,
                                const PointVelType& point_vel) {
  OutViewVelType out_vel;
  auto status =
      VelocityViewFrom(origin_loc, point_loc, origin_vel, point_vel, &out_vel);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityViewFrom.");

  return out_vel;
}

// Variant dispatch — two locations, VelocityViewFrom.
template <class ViewVelType, class OriginLocType, class PointLocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, ViewVelType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const VelocityTypes& origin_vel,
                        const VelocityTypes& point_vel, ViewVelType* out_vel) {
  Status status = Status::SUCCESS;
  std::visit(overloaded{[&status, &point_vel, &origin_loc, &point_loc,
                         out_vel](const auto& orig_vel) {
               std::visit(overloaded{[&status, &orig_vel, &origin_loc,
                                      &point_loc, out_vel](const auto& pt_vel) {
                            status =
                                VelocityViewFrom(origin_loc, point_loc,
                                                 orig_vel, pt_vel, out_vel);
                          }},
                          point_vel);
             }},
             origin_vel);
  return status;
}

// Variant dispatch — two locations, VelocityFrom.
template <
    class VelType, class OriginLocType, class PointLocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc,
                    const VelocityTypes& origin_vel,
                    const VelocityViewTypes& view_vel, VelType* out_vel) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_vel, &origin_loc, &point_loc,
           out_vel](const auto& orig_vel) {
            std::visit(
                overloaded{
                    [&status, &orig_vel, &origin_loc, &point_loc,
                     out_vel](const auto& vw_vel) {
                      status = VelocityFrom(origin_loc, point_loc, orig_vel,
                                            vw_vel, out_vel);
                    },
                },
                view_vel);
          },
      },
      origin_vel);
  return status;
}

// Variant dispatch — LocationViewTypes as point_loc.
template <class ViewVelType, class OriginLocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, ViewVelType>>>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const LocationViewTypes& point_loc,
                        const VelocityTypes& origin_vel,
                        const VelocityTypes& point_vel, ViewVelType* out_vel) {
  Status status = Status::SUCCESS;
  std::visit(overloaded{[&status, &origin_loc, &origin_vel, &point_vel,
                         out_vel](const auto& pt_loc) {
               std::visit(
                   overloaded{[&status, &origin_loc, &pt_loc, &point_vel,
                               out_vel](const auto& orig_vel) {
                     std::visit(
                         overloaded{[&status, &origin_loc, &pt_loc, &orig_vel,
                                     out_vel](const auto& pt_vel) {
                           status = VelocityViewFrom(origin_loc, pt_loc,
                                                     orig_vel, pt_vel, out_vel);
                         }},
                         point_vel);
                   }},
                   origin_vel);
             }},
             point_loc);
  return status;
}

template <
    class VelType, class OriginLocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const LocationViewTypes& point_loc,
                    const VelocityTypes& origin_vel,
                    const VelocityViewTypes& view_vel, VelType* out_vel) {
  Status status = Status::SUCCESS;
  std::visit(overloaded{[&status, &origin_loc, &origin_vel, &view_vel,
                         out_vel](const auto& pt_loc) {
               std::visit(
                   overloaded{[&status, &origin_loc, &pt_loc, &view_vel,
                               out_vel](const auto& orig_vel) {
                     std::visit(
                         overloaded{[&status, &origin_loc, &pt_loc, &orig_vel,
                                     out_vel](const auto& vw_vel) {
                           status = VelocityFrom(origin_loc, pt_loc, orig_vel,
                                                 vw_vel, out_vel);
                         }},
                         view_vel);
                   }},
                   origin_vel);
             }},
             point_loc);
  return status;
}

}  // namespace mutatio
