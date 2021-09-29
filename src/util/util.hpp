#pragma once

#include <cmath>
#include <string>
#include <utility>

namespace specni {
namespace util {

inline float DegToRad(float deg) { return (M_PI * deg) / 180; }

const std::string get_sign_deg(/* longitude */ double lon);
} // namespace util
} // namespace specni
