#include "mutatio/velocity_view_exchange.h"

#include "mutatio/velocity_exchange.h"

namespace mutatio {

Status VelocityViewFrom(const LlaLocation&, const EcefVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  *out = EcefVelocityView{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  return Status::SUCCESS;
}

// The location is accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const EcefVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  *out = EcefVelocity{origin.vx + view.vx, origin.vy + view.vy,
                      origin.vz + view.vz};
  return Status::SUCCESS;
}

// Cross-type EcefVelocityView: convert NED to ECEF, then subtract in ECEF.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, origin, ecef_point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const NedVelocity& point, EcefVelocityView* out) {
  EcefVelocity ecef_origin, ecef_point;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, ecef_point, out);
}

// Cross-type NedVelocityView: convert NED to ECEF, then use the ECEF+ECEF path.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, point, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, origin, ecef_point, out);
}

Status VelocityViewFrom(const LlaLocation& loc, const NedVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  return VelocityViewFrom(loc, ecef_origin, point, out);
}

// Computes the ECEF velocity difference and rotates it into the NED frame at
// the given location.
Status VelocityViewFrom(const LlaLocation& loc, const EcefVelocity& origin,
                        const EcefVelocity& point, NedVelocityView* out) {
  const EcefVelocity diff{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  NedVelocity ned_diff;
  auto stat = VelocityFrom(loc, diff, &ned_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = NedVelocityView{ned_diff.vnorth, ned_diff.veast, ned_diff.vdown};
  return Status::SUCCESS;
}

// Both velocities are already in the NED frame, so the view is their
// difference. The location is accepted for API consistency but not used.
Status VelocityViewFrom(const LlaLocation&, const NedVelocity& origin,
                        const NedVelocity& point, NedVelocityView* out) {
  *out =
      NedVelocityView{point.vnorth - origin.vnorth, point.veast - origin.veast,
                      point.vdown - origin.vdown};
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  const EcefVelocity ecef_point{origin.vx + view.vx, origin.vy + view.vy,
                                origin.vz + view.vz};
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, EcefVelocity* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{ecef_origin.vx + view.vx, ecef_origin.vy + view.vy,
                      ecef_origin.vz + view.vz};
  return Status::SUCCESS;
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const EcefVelocityView& view, NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ecef_point, out);
}

Status VelocityFrom(const LlaLocation& loc, const NedVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  NedVelocity ned_point;
  auto stat = VelocityFrom(loc, origin, view, &ned_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(loc, ned_point, out);
}

// Rotates the NED view into the ECEF frame and applies it to the origin.
Status VelocityFrom(const LlaLocation& loc, const EcefVelocity& origin,
                    const NedVelocityView& view, EcefVelocity* out) {
  const NedVelocity ned_view{view.vnorth, view.veast, view.vdown};
  EcefVelocity ecef_diff;
  auto stat = VelocityFrom(loc, ned_view, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{origin.vx + ecef_diff.vx, origin.vy + ecef_diff.vy,
                      origin.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// Both origin and view are in the NED frame; the result is their sum.
// The location is accepted for API consistency but not used.
Status VelocityFrom(const LlaLocation&, const NedVelocity& origin,
                    const NedVelocityView& view, NedVelocity* out) {
  *out = NedVelocity{origin.vnorth + view.vnorth, origin.veast + view.veast,
                     origin.vdown + view.vdown};
  return Status::SUCCESS;
}

// Two-location VelocityFrom — all arithmetic is performed in ECEF.

// No rotation needed; both locations are unused.
Status VelocityFrom(const LlaLocation&, const LlaLocation&,
                    const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out) {
  *out = EcefVelocity{origin.vx + view.vx, origin.vy + view.vy,
                      origin.vz + view.vz};
  return Status::SUCCESS;
}

// Add in ECEF, then rotate to NED at point_loc.
Status VelocityFrom(const LlaLocation&, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const EcefVelocityView& view,
                    NedVelocity* out) {
  const EcefVelocity ecef_point{origin.vx + view.vx, origin.vy + view.vy,
                                origin.vz + view.vz};
  return VelocityFrom(point_loc, ecef_point, out);
}

// Rotate NED view to ECEF at origin_loc, then add.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation&,
                    const EcefVelocity& origin, const NedVelocityView& view,
                    EcefVelocity* out) {
  EcefVelocity ecef_diff;
  auto stat = VelocityFrom(
      origin_loc, NedVelocity{view.vnorth, view.veast, view.vdown}, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{origin.vx + ecef_diff.vx, origin.vy + ecef_diff.vy,
                      origin.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// Rotate NED view to ECEF at origin_loc, add, then rotate to NED at point_loc.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const NedVelocityView& view,
                    NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(origin_loc, point_loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(point_loc, ecef_point, out);
}

// Rotate NED origin to ECEF at origin_loc, then add.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation&,
                    const NedVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out) {
  EcefVelocity ecef_origin;
  auto stat = VelocityFrom(origin_loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  *out = EcefVelocity{ecef_origin.vx + view.vx, ecef_origin.vy + view.vy,
                      ecef_origin.vz + view.vz};
  return Status::SUCCESS;
}

// Rotate NED origin to ECEF at origin_loc, add, then rotate to NED at
// point_loc.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const EcefVelocityView& view,
                    NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(origin_loc, point_loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(point_loc, ecef_point, out);
}

// Rotate NED origin and NED view to ECEF at origin_loc, then add.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation&,
                    const NedVelocity& origin, const NedVelocityView& view,
                    EcefVelocity* out) {
  EcefVelocity ecef_origin, ecef_diff;
  auto stat = VelocityFrom(origin_loc, origin, &ecef_origin);
  if (stat != Status::SUCCESS) return stat;
  stat = VelocityFrom(
      origin_loc, NedVelocity{view.vnorth, view.veast, view.vdown}, &ecef_diff);
  if (stat != Status::SUCCESS) return stat;
  *out =
      EcefVelocity{ecef_origin.vx + ecef_diff.vx, ecef_origin.vy + ecef_diff.vy,
                   ecef_origin.vz + ecef_diff.vz};
  return Status::SUCCESS;
}

// Rotate NED origin and view to ECEF at origin_loc, add, then rotate to NED at
// point_loc.
Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const NedVelocityView& view,
                    NedVelocity* out) {
  EcefVelocity ecef_point;
  auto stat = VelocityFrom(origin_loc, point_loc, origin, view, &ecef_point);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(point_loc, ecef_point, out);
}

// AerVelocityView — all arithmetic routes through AerVelocity.
// VelocityViewFrom: convert both inputs to AerVelocity, then subtract.
// VelocityFrom: convert origin to AerVelocity, add view, convert to output
// type.

namespace {

Status ToAerVelocity(const LlaLocation& origin_loc,
                     const LlaLocation& point_loc, const AerVelocity& in,
                     AerVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

Status ToAerVelocity(const LlaLocation& origin_loc,
                     const LlaLocation& point_loc, const EcefVelocity& in,
                     AerVelocity* out) {
  return VelocityFrom(origin_loc, point_loc, in, out);
}

Status ToAerVelocity(const LlaLocation& origin_loc,
                     const LlaLocation& point_loc, const NedVelocity& in,
                     AerVelocity* out) {
  return VelocityFrom(origin_loc, point_loc, in, out);
}

// PointToAerVelocity: interprets the input velocity as being in the NED frame
// at point_loc (rather than origin_loc).
Status PointToAerVelocity(const LlaLocation& origin_loc,
                          const LlaLocation& point_loc, const AerVelocity& in,
                          AerVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

Status PointToAerVelocity(const LlaLocation& origin_loc,
                          const LlaLocation& point_loc, const EcefVelocity& in,
                          AerVelocity* out) {
  return VelocityFrom(origin_loc, point_loc, in, out);
}

Status PointToAerVelocity(const LlaLocation& origin_loc,
                          const LlaLocation& point_loc, const NedVelocity& in,
                          AerVelocity* out) {
  EcefVelocity ecef;
  auto stat = VelocityFrom(point_loc, in, &ecef);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(origin_loc, point_loc, ecef, out);
}

Status FromAerVelocity(const LlaLocation& origin_loc,
                       const LlaLocation& point_loc, const AerVelocity& in,
                       AerVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

Status FromAerVelocity(const LlaLocation& origin_loc,
                       const LlaLocation& point_loc, const AerVelocity& in,
                       EcefVelocity* out) {
  return VelocityFrom(origin_loc, point_loc, in, out);
}

// Output is in the NED frame at point_loc to match the two-location convention.
Status FromAerVelocity(const LlaLocation& origin_loc,
                       const LlaLocation& point_loc, const AerVelocity& in,
                       NedVelocity* out) {
  EcefVelocity ecef;
  auto stat = VelocityFrom(origin_loc, point_loc, in, &ecef);
  if (stat != Status::SUCCESS) return stat;
  return VelocityFrom(point_loc, ecef, out);
}

}  // namespace

template <class OrigVel, class PointVel>
static Status VelocityViewFromAer(const LlaLocation& origin_loc,
                                  const LlaLocation& point_loc,
                                  const OrigVel& origin, const PointVel& point,
                                  AerVelocityView* out) {
  AerVelocity aer_origin, aer_point;
  auto stat = ToAerVelocity(origin_loc, point_loc, origin, &aer_origin);
  if (stat != Status::SUCCESS) return stat;
  stat = PointToAerVelocity(origin_loc, point_loc, point, &aer_point);
  if (stat != Status::SUCCESS) return stat;
  *out = AerVelocityView{aer_point.vazimuth - aer_origin.vazimuth,
                         aer_point.velevation - aer_origin.velevation,
                         aer_point.vrange - aer_origin.vrange};
  return Status::SUCCESS;
}

template <class OrigVel, class OutVel>
static Status VelocityFromAer(const LlaLocation& origin_loc,
                              const LlaLocation& point_loc,
                              const OrigVel& origin,
                              const AerVelocityView& view, OutVel* out) {
  AerVelocity aer_origin;
  auto stat = ToAerVelocity(origin_loc, point_loc, origin, &aer_origin);
  if (stat != Status::SUCCESS) return stat;
  const AerVelocity aer_point{aer_origin.vazimuth + view.vazimuth,
                              aer_origin.velevation + view.velevation,
                              aer_origin.vrange + view.vrange};
  return FromAerVelocity(origin_loc, point_loc, aer_point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const AerVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const EcefVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const AerVelocity& origin,
                        const NedVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const AerVelocity& point,
                        AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const EcefVelocity& point,
                        AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc,
                        const EcefVelocity& origin, const NedVelocity& point,
                        AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const AerVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const EcefVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityViewFrom(const LlaLocation& origin_loc,
                        const LlaLocation& point_loc, const NedVelocity& origin,
                        const NedVelocity& point, AerVelocityView* out) {
  return VelocityViewFromAer(origin_loc, point_loc, origin, point, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const AerVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const EcefVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    AerVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    EcefVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

Status VelocityFrom(const LlaLocation& origin_loc, const LlaLocation& point_loc,
                    const NedVelocity& origin, const AerVelocityView& view,
                    NedVelocity* out) {
  return VelocityFromAer(origin_loc, point_loc, origin, view, out);
}

}  // namespace mutatio
