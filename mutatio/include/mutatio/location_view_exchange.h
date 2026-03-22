#pragma once

#include <cmath>

#include "GeographicLib/Constants.hpp"
#include "GeographicLib/Geodesic.hpp"

#include "mutatio/location.h"
#include "mutatio/location_exchange.h"
#include "mutatio/location_view.h"
#include "mutatio/status.h"
#include "mutatio/units.h"

namespace mutatio {

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                NedLocationView* out) {
  LlaLocation origin_lla, point_lla;
  Status stat;

  stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  stat = LocationFrom(point, &point_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  Meter s12;
  Degree azi1, azi2;
  earth.Inverse(origin_lla.lat, origin_lla.lon, point_lla.lat, point_lla.lon,
                s12, azi1, azi2);

  const auto north = s12 * std::cos(azi1 * GeographicLib::Constants::degree());
  const auto east  = s12 * std::sin(azi1 * GeographicLib::Constants::degree());
  const auto down  = origin_lla.alt - point_lla.alt;

  *out = NedLocationView{north, east, down};

  return Status::SUCCESS;
}

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                AerLocationView* out) {
  LlaLocation origin_lla, point_lla;
  Status stat;

  stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  stat = LocationFrom(point, &point_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  Meter s12;
  Degree azi1, azi2;
  earth.Inverse(origin_lla.lat, origin_lla.lon, point_lla.lat, point_lla.lon,
                s12, azi1, azi2);

  const Meter h12  = (point_lla.alt - origin_lla.alt);
  const auto elev  = std::atan2(h12, s12) / GeographicLib::Constants::degree();
  const auto range = std::sqrt((s12 * s12) + (h12 * h12));

  *out = AerLocationView{azi1, elev, range};

  return Status::SUCCESS;
}

template <class OriginType, class PointType>
Status ViewFrom(const OriginType& origin, const PointType& point,
                EcefLocationView* out) {
  EcefLocation origin_ecef, point_ecef;
  Status stat;

  stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  stat = LocationFrom(point, &point_ecef);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const auto x = point_ecef.x - origin_ecef.x;
  const auto y = point_ecef.y - origin_ecef.y;
  const auto z = point_ecef.z - origin_ecef.z;

  *out = EcefLocationView{x, y, z};

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
  const auto deg_to_rad = GeographicLib::Constants::degree();

  const auto s     = std::sqrt((in.north * in.north) + (in.east * in.east));
  const auto h     = -in.down;
  const auto range = std::sqrt((s * s) + (h * h));
  const auto elev  = std::atan2(h, s) / deg_to_rad;
  const auto azi   = std::atan2(in.north, in.east) / deg_to_rad;

  *out = AerLocationView{azi, elev, range};

  return Status::SUCCESS;
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const NedLocationView& in,
                EcefLocationView* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  const auto deg_to_rad = GeographicLib::Constants::degree();
  const Meter s12   = std::sqrt((in.north * in.north) + (in.east * in.east));
  const Degree azi1 = std::atan2(in.north, in.east) / deg_to_rad;

  Degree lat2, lon2, azi2;
  earth.Direct(origin_lla.lat, origin_lla.lon, azi1, s12, lat2, lon2, azi2);
  LlaLocation point_lla{lat2, lon2, origin_lla.alt - in.down};

  return ViewFrom(origin_lla, point_lla, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const AerLocationView& in,
                NedLocationView* out) {
  const auto deg_to_rad = GeographicLib::Constants::degree();

  const auto s     = in.range * std::cos(in.elevation * deg_to_rad);
  const auto north = s * std::cos(in.azimuth * deg_to_rad);
  const auto east  = s * std::sin(in.azimuth * deg_to_rad);
  const auto down  = -in.range * std::sin(in.elevation * deg_to_rad);

  *out = NedLocationView{north, east, down};

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
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  const auto deg_to_rad = GeographicLib::Constants::degree();
  const Meter s12       = in.range * std::cos(in.elevation * deg_to_rad);

  Degree lat2, lon2, azi2;
  earth.Direct(origin_lla.lat, origin_lla.lon, in.azimuth, s12, lat2, lon2,
               azi2);

  const auto h = in.range * std::sin(in.elevation * deg_to_rad);
  LlaLocation point_lla{lat2, lon2, origin_lla.alt + h};

  return ViewFrom(origin_lla, point_lla, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const EcefLocationView& in,
                NedLocationView* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  EcefLocation point_ecef{origin_ecef.x + in.x, origin_ecef.y + in.y,
                          origin_ecef.z + in.z};

  return ViewFrom(origin_ecef, point_ecef, out);
}

template <class OriginType>
Status ViewFrom(const OriginType& origin, const EcefLocationView& in,
                AerLocationView* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) {
    return stat;
  }

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
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  const auto deg_to_rad = GeographicLib::Constants::degree();
  const Meter s12 =
      std::sqrt((view.north * view.north) + (view.east * view.east));
  const Degree azi1 = std::atan2(view.north, view.east) / deg_to_rad;

  Degree lat2, lon2, azi2;
  earth.Direct(origin_lla.lat, origin_lla.lon, azi1, s12, lat2, lon2, azi2);
  LlaLocation point_lla{lat2, lon2, origin_lla.alt - view.down};

  return LocationFrom(point_lla, out);
}

template <class OriginType, class PointType>
Status LocationFrom(const OriginType& origin, const AerLocationView& view,
                    PointType* out) {
  LlaLocation origin_lla;
  auto stat = LocationFrom(origin, &origin_lla);
  if (stat != Status::SUCCESS) {
    return stat;
  }

  const GeographicLib::Geodesic earth(
      GeographicLib::Constants::WGS84_a() + origin_lla.alt,
      GeographicLib::Constants::WGS84_f());  // TODO: Adjust flattening.

  const auto deg_to_rad = GeographicLib::Constants::degree();
  const Meter s12       = view.range * std::cos(view.elevation * deg_to_rad);

  Degree lat2, lon2, azi2;
  earth.Direct(origin_lla.lat, origin_lla.lon, view.azimuth, s12, lat2, lon2,
               azi2);

  const auto h = view.range * std::sin(view.elevation * deg_to_rad);
  LlaLocation point_lla{lat2, lon2, origin_lla.alt + h};

  return LocationFrom(point_lla, out);
}

template <class OriginType, class PointType>
Status LocationFrom(const OriginType& origin, const EcefLocationView& view,
                    PointType* out) {
  EcefLocation origin_ecef;
  auto stat = LocationFrom(origin, &origin_ecef);
  if (stat != Status::SUCCESS) {
    return stat;
  }

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
