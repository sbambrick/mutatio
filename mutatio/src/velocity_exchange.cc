#include "mutatio/velocity_exchange.h"

namespace mutatio {

// EcefVelocity is frame-independent; this is an identity copy.
Status VelocityFrom(const EcefVelocity& in, EcefVelocity* out) {
  *out = in;
  return Status::SUCCESS;
}

}  // namespace mutatio
