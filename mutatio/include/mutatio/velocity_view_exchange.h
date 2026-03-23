#pragma once

#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/status.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/velocity.h"
#include "mutatio/velocity_view.h"

namespace mutatio {

// EcefVelocityView — NED velocities are converted to ECEF first.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out);
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out);
Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out);
Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out);

// NedVelocityView — a location is required to define the NED frame.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out);
Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out);

// Cross-type NedVelocityView — NED velocities are converted to ECEF first.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out);
Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out);

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out);
Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out);

// Two-location VelocityFrom — origin_loc defines the NED frame for the origin
// velocity and view; point_loc defines the NED frame for the output velocity.
// All arithmetic is performed in ECEF.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const EcefVelocityView& view,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const NedVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const NedVelocityView& view,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const EcefVelocityView& view,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const NedVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const NedVelocityView& view,
                    NedVelocity* out);

// AerVelocityView requires two locations; single-location calls always return
// ERROR.
template <class LocType, class OrigVel, class PointVel>
inline Status VelocityViewFrom(const LocType&, const OrigVel&, const PointVel&,
                               AerVelocityView*) {
  return Status::ERROR;
}
template <class LocType, class OrigVel, class OutVel>
inline Status VelocityFrom(const LocType&, const OrigVel&,
                           const AerVelocityView&, OutVel*) {
  return Status::ERROR;
}

// Two-location AerVelocityView VelocityViewFrom — origin and point velocities
// are converted to AerVelocity using the two-location Jacobian, then
// subtracted.
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const AerVelocity& point, AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const EcefVelocity& point, AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const NedVelocity& point, AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const AerVelocity& point,
                        AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const EcefVelocity& point,
                        AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const NedVelocity& point,
                        AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const AerVelocity& point, AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const EcefVelocity& point, AerVelocityView* out);
Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const NedVelocity& point, AerVelocityView* out);

// Two-location AerVelocityView VelocityFrom — origin velocity is converted to
// AerVelocity, the view is added, then the result is converted to the output
// type.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out);
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out);

// LocType overloads — convert any location type to LlaLocation, then call the
// specific overload above.
template <class LocType>
Status VelocityViewFrom(const LocType& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const EcefVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const NedVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const NedVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const NedVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const EcefVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityViewFrom(const LocType& loc, const NedVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(lla, origin, point, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

template <class LocType>
Status VelocityFrom(const LocType& loc, const NedVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  LlaLocation lla;
  auto stat = LocationFrom(loc, &lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(lla, origin, view, out);
}

// Two-location LocType overloads.
template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

// Two-location AerVelocityView LocType templates.
template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const AerVelocity& origin, const AerVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const AerVelocity& origin, const EcefVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const AerVelocity& origin, const NedVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin, const AerVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin, const EcefVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const EcefVelocity& origin, const NedVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const NedVelocity& origin, const AerVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const NedVelocity& origin, const EcefVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityViewFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc,
                        const NedVelocity& origin, const NedVelocity& point,
                        AerVelocityView* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(origin_lla, point_lla, origin, point, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& origin,
                    const AerVelocityView& view, AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& origin,
                    const AerVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const AerVelocity& origin,
                    const AerVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const AerVelocityView& view, AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const AerVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const EcefVelocity& origin,
                    const AerVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const AerVelocityView& view, AerVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const AerVelocityView& view, EcefVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

template <class OriginLocType, class PointLocType>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const NedVelocity& origin,
                    const AerVelocityView& view, NedVelocity* out) {
  LlaLocation origin_lla, point_lla;
  auto stat = LocationFrom(origin_loc, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point_loc, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_lla, point_lla, origin, view, out);
}

// Throwing variant — two locations.
template <class OutVelType, class OriginType, class ViewType,
          class OriginLocType, class PointLocType>
OutVelType VelocityFrom(const OriginLocType& origin_loc,
                        const PointLocType& point_loc, const OriginType& origin,
                        const ViewType& view) {
  OutVelType out;
  auto status = VelocityFrom(origin_loc, point_loc, origin, view, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out;
}

// Variant dispatch — two locations.
template <
    class VelType, class OriginLocType, class PointLocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const OriginLocType& origin_loc,
                    const PointLocType& point_loc, const VelocityTypes& origin,
                    const VelocityViewTypes& view_variant, VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, &origin_loc, &point_loc,
           out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, &origin_loc, &point_loc,
                     out](const auto& view) {
                      status =
                          VelocityFrom(origin_loc, point_loc, orig, view, out);
                    },
                },
                view_variant);
          },
      },
      origin);
  return status;
}

// Throwing variants — with location.
template <class OutViewType, class OriginType, class PointType,
          class OriginLocType, class PointLocType>
OutViewType VelocityViewFrom(const OriginLocType& origin_loc,
                             const PointLocType& point_loc,
                             const OriginType& origin, const PointType& point) {
  OutViewType out;
  auto status = VelocityViewFrom(origin_loc, point_loc, origin, point, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityViewFrom.");

  return out;
}

template <class OutViewType, class OriginType, class PointType, class LocType>
OutViewType VelocityViewFrom(const LocType& loc, const OriginType& origin,
                             const PointType& point) {
  OutViewType out;
  auto status = VelocityViewFrom(loc, origin, point, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityViewFrom.");

  return out;
}

template <class OutVelType, class OriginType, class ViewType, class LocType>
OutVelType VelocityFrom(const LocType& loc, const OriginType& origin,
                        const ViewType& view) {
  OutVelType out;
  auto status = VelocityFrom(loc, origin, view, &out);

  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in VelocityFrom.");

  return out;
}

// Variant dispatch — with location.
template <class ViewType, class LocType,
          class = std::enable_if_t<
              is_one_of_variants_types<VelocityViewTypes, ViewType>>>
Status VelocityViewFrom(const LocType& loc, const VelocityTypes& origin,
                        const VelocityTypes& point, ViewType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &point, &loc, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, &loc, out](const auto& pt) {
                      status = VelocityViewFrom(loc, orig, pt, out);
                    },
                },
                point);
          },
      },
      origin);
  return status;
}

template <
    class VelType, class LocType,
    class = std::enable_if_t<is_one_of_variants_types<VelocityTypes, VelType>>>
Status VelocityFrom(const LocType& loc, const VelocityTypes& origin,
                    const VelocityViewTypes& view_variant, VelType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, &loc, out](const auto& orig) {
            std::visit(
                overloaded{
                    [&status, &orig, &loc, out](const auto& view) {
                      status = VelocityFrom(loc, orig, view, out);
                    },
                },
                view_variant);
          },
      },
      origin);
  return status;
}

}  // namespace mutatio
