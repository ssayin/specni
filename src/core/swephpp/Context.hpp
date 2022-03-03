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

struct ContextInit {
  ContextInit();
};

}; // namespace swe
}; // namespace core
}; // namespace specni
