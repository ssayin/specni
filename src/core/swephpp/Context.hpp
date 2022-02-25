#pragma once

#include <string>

namespace specni {
namespace core {
namespace swe {

struct Context {
  Context();
  Context(std::string eph_path);
  ~Context();
};

extern Context &ctx;

static struct ContextInit {
  ContextInit();
  ~ContextInit();
} ctx_init;

}; // namespace swe
}; // namespace core
}; // namespace specni
