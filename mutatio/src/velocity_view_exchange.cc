#include "mutatio/velocity_view_exchange.h"

#include <cmath>
#include <limits>

#include "GeographicLib/Constants.hpp"

#include "mutatio/location_view_exchange.h"
#include "mutatio/velocity_exchange.h"

namespace mutatio {

namespace {

// Internal intermediate types used for NED/AER computations.
struct NedVel {
  double vnorth, veast, vdown;
};
struct AerVel {
  double vazimuth, velevation, vrange;
};

// Rotates an EcefVelocity into the NED frame at loc.
Status EcefToNed(const LlaLocation& loc, const EcefVelocity& in, NedVel* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);
  *out               = NedVel{
      -sin_lat * cos_lon * in.vx - sin_lat * sin_lon * in.vy + cos_lat * in.vz,
      -sin_lon * in.vx + cos_lon * in.vy,
      -cos_lat * cos_lon * in.vx - cos_lat * sin_lon * in.vy - sin_lat * in.vz,
  };
  return Status::SUCCESS;
}

// Rotates a NedVel back into the ECEF frame (transpose of EcefToNed).
Status NedToEcef(const LlaLocation& loc, const NedVel& in, EcefVelocity* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto sin_lat = std::sin(loc.lat * deg);
  const auto cos_lat = std::cos(loc.lat * deg);
  const auto sin_lon = std::sin(loc.lon * deg);
  const auto cos_lon = std::cos(loc.lon * deg);
  *out               = EcefVelocity{
      -sin_lat * cos_lon * in.vnorth - sin_lon * in.veast -
          cos_lat * cos_lon * in.vdown,
      -sin_lat * sin_lon * in.vnorth + cos_lon * in.veast -
          cos_lat * sin_lon * in.vdown,
      cos_lat * in.vnorth - sin_lat * in.vdown,
  };
  return Status::SUCCESS;
}

// AER-to-NED Jacobian: converts AerVel at a given AER position to NedVel.
Status AerToNed(const AerLocationView& pos, const AerVel& in, NedVel* out) {
  const auto deg    = GeographicLib::Constants::degree();
  const auto vel_az = in.vazimuth * deg;
  const auto vel_el = in.velevation * deg;

  const auto cos_az = std::cos(pos.azimuth * deg);
  const auto sin_az = std::sin(pos.azimuth * deg);
  const auto cos_el = std::cos(pos.elevation * deg);
  const auto sin_el = std::sin(pos.elevation * deg);

  *out = NedVel{
      cos_el * cos_az * in.vrange - pos.range * sin_el * cos_az * vel_el -
          pos.range * cos_el * sin_az * vel_az,
      cos_el * sin_az * in.vrange - pos.range * sin_el * sin_az * vel_el +
          pos.range * cos_el * cos_az * vel_az,
      -sin_el * in.vrange - pos.range * cos_el * vel_el,
  };
  return Status::SUCCESS;
}

// NED-to-AER inverse Jacobian. Returns ERROR on singularity (r ≈ 0 or el ≈
// ±90°).
Status NedToAer(const AerLocationView& pos, const NedVel& in, AerVel* out) {
  const auto deg     = GeographicLib::Constants::degree();
  const auto cos_az  = std::cos(pos.azimuth * deg);
  const auto sin_az  = std::sin(pos.azimuth * deg);
  const auto cos_el  = std::cos(pos.elevation * deg);
  const auto sin_el  = std::sin(pos.elevation * deg);
  const auto epsilon = std::numeric_limits<double>::epsilon() * 1e6;

  if (pos.range < epsilon) return Status::ERROR;
  if (std::abs(cos_el) < epsilon) return Status::ERROR;

  const auto vrange = cos_el * cos_az * in.vnorth + cos_el * sin_az * in.veast -
                      sin_el * in.vdown;
  const auto vel_el_rad = (-sin_el * cos_az * in.vnorth -
                           sin_el * sin_az * in.veast - cos_el * in.vdown) /
                          pos.range;
  const auto vel_az_rad =
      (-sin_az * in.vnorth + cos_az * in.veast) / (pos.range * cos_el);

  *out = AerVel{vel_az_rad / deg, vel_el_rad / deg, vrange};
  return Status::SUCCESS;
}

// Converts an EcefVelocity to AerVel using the two-location Jacobian.
// ECEF → NED at origin_loc → AER.
Status EcefToAer(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                 const EcefVelocity& in, AerVel* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  NedVel ned;
  stat = EcefToNed(origin_loc, in, &ned);
  if (stat != Status::SUCCESS) return stat;

  return NedToAer(aer_pos, ned, out);
}

// Converts an AerVel to EcefVelocity using the two-location Jacobian.
// AER → NED at origin_loc → ECEF.
Status AerToEcef(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                 const AerVel& in, EcefVelocity* out) {
  AerLocationView aer_pos;
  auto stat = ViewFrom(origin_loc, point_loc, &aer_pos);
  if (stat != Status::SUCCESS) return stat;

  NedVel ned;
  stat = AerToNed(aer_pos, in, &ned);
  if (stat != Status::SUCCESS) return stat;

  return NedToEcef(origin_loc, ned, out);
}

// Rotates a NedVelocityView into the ECEF frame.
Status NedViewToEcef(const LlaLocation& loc, const NedVelocityView& in,
                     EcefVelocity* out) {
  return NedToEcef(loc, NedVel{in.vnorth, in.veast, in.vdown}, out);
}

}  // namespace

// Two-location VelocityViewFrom — EcefVelocityView: both locations unused.
Status VelocityViewFrom(const LlaLocation&, const LlaLocation&,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        EcefVelocityView* out_vel) {
  *out_vel = EcefVelocityView{point_vel.vx - origin_vel.vx,
                              point_vel.vy - origin_vel.vy,
                              point_vel.vz - origin_vel.vz};
  return Status::SUCCESS;
}

// Two-location VelocityViewFrom — NedVelocityView: origin_loc defines the NED
// frame; point_loc unused.
Status VelocityViewFrom(const LlaLocation& origin_loc, const LlaLocation&,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        NedVelocityView* out_vel) {
  const EcefVelocity diff{point_vel.vx - origin_vel.vx,
                          point_vel.vy - origin_vel.vy,
                          point_vel.vz - origin_vel.vz};
  NedVel ned;
  auto stat = EcefToNed(origin_loc, diff, &ned);
  if (stat != Status::SUCCESS) return stat;
  *out_vel = NedVelocityView{ned.vnorth, ned.veast, ned.vdown};
  return Status::SUCCESS;
}

// Two-location VelocityFrom — all arithmetic is performed in ECEF.

// No rotation needed; both locations unused.
Status VelocityFrom(const LlaLocation&, const LlaLocation&,
                    const EcefVelocity& origin_vel,
                    const EcefVelocityView& view_vel, EcefVelocity* out_vel) {
  *out_vel =
      EcefVelocity{origin_vel.vx + view_vel.vx, origin_vel.vy + view_vel.vy,
                   origin_vel.vz + view_vel.vz};
  return Status::SUCCESS;
}

// Rotate NED view to ECEF at origin_loc, then add. point_loc unused.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation&,
                    const EcefVelocity& origin_vel,
                    const NedVelocityView& view_vel, EcefVelocity* out_vel) {
  EcefVelocity ecef_diff;
  auto stat = NedViewToEcef(origin_loc, view_vel, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out_vel =
      EcefVelocity{origin_vel.vx + ecef_diff.vx, origin_vel.vy + ecef_diff.vy,
                   origin_vel.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// AerVelocityView — arithmetic routes through AerVel.

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin_vel,
                        const EcefVelocity& point_vel,
                        AerVelocityView* out_vel) {
  AerVel aer_origin, aer_point;
  auto stat = EcefToAer(origin_loc, point_loc, origin_vel, &aer_origin);
  if (stat != Status::SUCCESS) return stat;
  stat = EcefToAer(origin_loc, point_loc, point_vel, &aer_point);
  if (stat != Status::SUCCESS) return stat;
  *out_vel = AerVelocityView{aer_point.vazimuth - aer_origin.vazimuth,
                             aer_point.velevation - aer_origin.velevation,
                             aer_point.vrange - aer_origin.vrange};
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin_vel,
                    const AerVelocityView& view_vel, EcefVelocity* out_vel) {
  AerVel aer_origin;
  auto stat = EcefToAer(origin_loc, point_loc, origin_vel, &aer_origin);
  if (stat != Status::SUCCESS) return stat;
  const AerVel aer_point{aer_origin.vazimuth + view_vel.vazimuth,
                         aer_origin.velevation + view_vel.velevation,
                         aer_origin.vrange + view_vel.vrange};
  return AerToEcef(origin_loc, point_loc, aer_point, out_vel);
}

}  // namespace mutatio
