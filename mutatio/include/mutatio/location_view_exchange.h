#pragma once

#include <cmath>
#include <stdexcept>

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/location_view.h"
#include "mutatio/status.h"
#include "mutatio/units.h"
#include "mutatio/variant_overloaded.h"
#include "mutatio/wgs84.h"

namespace mutatio {

namespace internal {

// Compute NED view between two LlaLocations (uses Geodesic.Inverse).
Status ViewFromLla(const LlaLocation& origin, const LlaLocation& point,
                   NedLocationView* out);

// Compute AER view between two LlaLocations (uses Geodesic.Inverse).
Status ViewFromLla(const LlaLocation& origin, const LlaLocation& point,
                   AerLocationView* out);

// Compute target LlaLocation from origin + NED offset (uses Geodesic.Direct).
Status PointLlaFromNed(const LlaLocation& origin, const NedLocationView& ned,
                       LlaLocation* out);

// Compute target LlaLocation from origin + AER offset (uses Geodesic.Direct).
Status PointLlaFromAer(const LlaLocation& origin, const AerLocationView& aer,
                       LlaLocation* out);

}  // namespace internal

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                NedLocationView* out) {
  LlaLocation origin_lla, point_lla;
  Status stat;
  stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return internal::ViewFromLla(origin_lla, point_lla, out);
}

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                AerLocationView* out) {
  LlaLocation origin_lla, point_lla;
  Status stat;
  stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return internal::ViewFromLla(origin_lla, point_lla, out);
}

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                EcefLocationView* out) {
  EcefLocation origin_ecef, point_ecef;
  Status stat;
  stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) return stat;
  stat = LocationFrom(point, &point_ecef);
  if (stat != Status::SUCCESS) return stat;
  const auto x = point_ecef.x - origin_ecef.x;
  const auto y = point_ecef.y - origin_ecef.y;
  const auto z = point_ecef.z - origin_ecef.z;
  *out         = EcefLocationView{x, y, z};
  return Status::SUCCESS;
}

template <class OutViewType, class OriginType, class PointType>
OutViewType ViewFrom(const OriginType& origin, const PointType& point) {
  OutViewType out;
  auto status = ViewFrom(origin, point, &out);
  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in LocationFrom.");
  return out;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const NedLocationView& in,
                NedLocationView* out) {
  *out = in;
  return Status::SUCCESS;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const NedLocationView& in,
                AerLocationView* out) {
  const auto s     = std::sqrt((in.north * in.north) + (in.east * in.east));
  const auto h     = -in.down;
  const auto range = std::sqrt((s * s) + (h * h));
  const auto elev  = std::atan2(h, s) / kDegToRad;
  const auto azi   = std::atan2(in.east, in.north) / kDegToRad;
  *out             = AerLocationView{azi, elev, range};
  return Status::SUCCESS;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const NedLocationView& in,
                EcefLocationView* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  LlaLocation point_lla;
  stat = internal::PointLlaFromNed(origin_lla, in, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return ViewFrom(origin_lla, point_lla, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const AerLocationView& in,
                NedLocationView* out) {
  const auto s     = in.range * std::cos(in.elevation * kDegToRad);
  const auto north = s * std::cos(in.azimuth * kDegToRad);
  const auto east  = s * std::sin(in.azimuth * kDegToRad);
  const auto down  = -in.range * std::sin(in.elevation * kDegToRad);
  *out             = NedLocationView{north, east, down};
  return Status::SUCCESS;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const AerLocationView& in,
                AerLocationView* out) {
  *out = in;
  return Status::SUCCESS;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const AerLocationView& in,
                EcefLocationView* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  LlaLocation point_lla;
  stat = internal::PointLlaFromAer(origin_lla, in, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return ViewFrom(origin_lla, point_lla, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const EcefLocationView& in,
                NedLocationView* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) return stat;
  EcefLocation point_ecef{origin_ecef.x + in.x, origin_ecef.y + in.y,
                          origin_ecef.z + in.z};
  return ViewFrom(origin_ecef, point_ecef, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const EcefLocationView& in,
                AerLocationView* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) return stat;
  EcefLocation point_ecef{origin_ecef.x + in.x, origin_ecef.y + in.y,
                          origin_ecef.z + in.z};
  return ViewFrom(origin_ecef, point_ecef, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const EcefLocationView& in,
                EcefLocationView* out) {
  *out = in;
  return Status::SUCCESS;
}

template <class OriginType, class PointType>
Status LocationFrom(const OriginType& origin, const NedLocationView& view,
                    PointType* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  LlaLocation point_lla;
  stat = internal::PointLlaFromNed(origin_lla, view, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return LocationFrom(point_lla, out);
}

template <class OriginType, class PointType>
Status LocationFrom(const OriginType& origin, const AerLocationView& view,
                    PointType* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) return stat;
  LlaLocation point_lla;
  stat = internal::PointLlaFromAer(origin_lla, view, &point_lla);
  if (stat != Status::SUCCESS) return stat;
  return LocationFrom(point_lla, out);
}

template <class OriginType, class PointType>
Status LocationFrom(const OriginType& origin, const EcefLocationView& view,
                    PointType* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) return stat;
  EcefLocation point_ecef{origin_ecef.x + view.x, origin_ecef.y + view.y,
                          origin_ecef.z + view.z};
  return LocationFrom(point_ecef, out);
}

template <class OutLocType, class OriginType, class ViewType>
OutLocType LocationFrom(const OriginType& origin, const ViewType& view) {
  OutLocType out;
  auto status = LocationFrom(origin, view, &out);
  if (status == Status::ERROR)
    throw std::invalid_argument("ERROR in LocationFrom.");
  return out;
}

template <class OriginType, class PointType,
          class = std::enable_if_t<
              is_one_of_variants_types<LocationTypes, PointType>>>
Status LocationFrom(const OriginType& origin,
                    const LocationViewTypes& view_variant, PointType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &origin, out](const auto& view) {
            status = LocationFrom(origin, view, out);
          },
      },
      view_variant);
  return status;
}

template <class PointType, class = std::enable_if_t<is_one_of_variants_types<
                               LocationTypes, PointType>>>
Status LocationFrom(const LocationTypes& origin_variant,
                    const LocationViewTypes& view_variant, PointType* out) {
  Status status = Status::SUCCESS;
  std::visit(
      overloaded{
          [&status, &view_variant, out](const auto& origin) {
            status = LocationFrom(origin, view_variant, out);
          },
      },
      origin_variant);
  return status;
}

}  // namespace mutatio
