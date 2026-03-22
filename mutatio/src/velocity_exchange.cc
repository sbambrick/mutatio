#include "mutatio/velocity_exchange.h"

namespace mutatio {

Status VelocityFrom(const EcefVelocity &in, EcefVelocity *out) {
  *out = in;
  return Status::SUCCESS;
}

}  // namespace mutatio
