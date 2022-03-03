#include "Context.hpp"

// clang-format off
#include "swephexp.h"
#include <iostream>
// clang-format on

namespace specni {
namespace core {
namespace swe {

Context::Context() { swe_set_ephe_path(NULL); }
Context::Context(std::string ephe_path) { swe_set_ephe_path(ephe_path.data()); }
Context::~Context() { swe_close(); }

ContextInit::ContextInit() { static Context ctx; }

}; // namespace swe

}; // namespace core

}; // namespace specni
