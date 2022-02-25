#include "Context.hpp"

// clang-format off
#include "swephexp.h"
// clang-format on

namespace specni {
namespace core {
namespace swe {

static int counter;
static std::aligned_storage_t<sizeof(Context),
                              std::alignment_of<Context>::value>
    ctx_buf;

Context &ctx = reinterpret_cast<Context &>(ctx_buf);

Context::Context() { swe_set_ephe_path(NULL); }
Context::Context(std::string ephe_path) { swe_set_ephe_path(ephe_path.data()); }
Context::~Context() { swe_close(); }

ContextInit::ContextInit() {
  if (counter++ == 0)
    new (&ctx) Context();
}

ContextInit::~ContextInit() {
  if (--counter == 0)
    (&ctx)->~Context();
}

}; // namespace swe

}; // namespace core

}; // namespace specni
