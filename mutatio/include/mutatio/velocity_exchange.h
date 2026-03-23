#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"

namespace mutatio {

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& in,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& in,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& in,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& in,
                    NedVelocity* out);

// Two-location VelocityFrom — origin_loc defines the NED frame of the input;
// point_loc defines the NED frame of the output. All arithmetic is in ECEF.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& in, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& in, NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& in, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& in, NedVelocity* out);

// Two-location AerVelocity overloads — origin_loc and point_loc together define
// the AER geometry (range, azimuth, elevation) used for the Jacobian.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& in, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& in, NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& in, AerVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& in, AerVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& in, AerVelocity* out);

// LocType overloads convert the location to LlaLocation, then call the
// specific overload above. This allows any LocationTypes value to be passed.
template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& in,
                    NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, in, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& in,
                    EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, in, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& in,
                    EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, in, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& in,
                    NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, in, out);
}

// AerVelocity requires two locations; single-location calls always return
// ERROR.
template <class LocType>
inline Status VelocityFrom(const LocType&, const AerVelocity&, EcefVelocity*) {
  return Status::ERROR;
}
template <class LocType>
inline Status VelocityFrom(const LocType&, const AerVelocity&, NedVelocity*) {
  return Status::ERROR;
}
template <class LocType>
inline Status VelocityFrom(const LocType&, const AerVelocity&, AerVelocity*) {
  return Status::ERROR;
}
template <class LocType>
inline Status VelocityFrom(const LocType&, const EcefVelocity&, AerVelocity*) {
  return Status::ERROR;
}
template <class LocType>
inline Status VelocityFrom(const LocType&, const NedVelocity&, AerVelocity*) {
  return Status::ERROR;
}

// Two-location LocType templates — convert both locations to LlaLocation, then
// delegate to the specific two-location overload above.
template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& in,
                    EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& in,
                    NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& in,
                    EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& in,
                    NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& in,
                    EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& in,
                    NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& in,
                    AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& in,
                    AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& in,
                    AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, in, out);
}

// Variant dispatch — dispatches over VelocityTypes with a location.
template <
    class VelType, class LocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const LocType& loc, const VelocityTypes& in_vel,
                    VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &loc, out](const auto& in) {
            status = VelocityFrom(loc, in, out);
          },
      },
      in_vel);
  return status;
}

// Two-location variant dispatch — dispatches over VelocityTypes with two
// locations.
template <
    class VelType, class OriginLocType, class PointLocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const VelocityTypes& in_vel,
                    VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &origin_loc, &point_loc, out](const auto& in) {
            status = VelocityFrom(origin_loc, point_loc, in, out);
          },
      },
      in_vel);
  return status;
}

// Throwing variant — frame conversions requiring a location.
template <class OutVelType, class InVelType, class LocType>
OutVelType VelocityFrom(const LocType& loc, const InVelType& in) {
  OutVelType out;
  auto status = VelocityFrom(loc, in, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out;
}

// Two-location throwing variant.
template <class OutVelType, class InVelType, class OriginLocType,
          class PointLocType>
OutVelType VelocityFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc, const InVelType& in) {
  OutVelType out;
  auto status = VelocityFrom(origin_loc, point_loc, in, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out;
}

}  // namespace mutatio
