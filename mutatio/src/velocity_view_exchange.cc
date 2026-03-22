#include "mutatio/velocity_view_exchange.h"

namespace mutatio {

Status VelocityViewFrom(const EcefVelocity& origin, const EcefVelocity& point,
                        EcefVelocityView* out) {
  *out = EcefVelocityView{point.vx - origin.vx, point.vy - origin.vy,
                          point.vz - origin.vz};
  return Status::SUCCESS;
}

Status VelocityFrom(const EcefVelocity& origin, const EcefVelocityView& view,
                    EcefVelocity* out) {
  *out = EcefVelocity{origin.vx + view.vx, origin.vy + view.vy,
                      origin.vz + view.vz};
  return Status::SUCCESS;
}

}  // namespace mutatio
