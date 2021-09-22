#pragma once

#include <cmath>
#include <string>
#include <utility>

namespace specni {
namespace util {

const std::string get_sign_string_by_longitude(double lon);
const std::pair<std::string, double> get_sign_deg(/* longitude */ double lon);
} // namespace util
} // namespace specni
