#include "Context.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni::core::swe {

Context::Context() { swe_set_ephe_path("ephe"); }

Context::Context(std::string eph_path) { swe_set_ephe_path(eph_path.data()); }

Context::~Context() { swe_close(); }

ContextInit::ContextInit() { static Context ctx; }

} // namespace specni
