#pragma once

#include <array>
#include <cstdint>

namespace specni::core::swe {

namespace detail {
constexpr uint8_t
		EphDataSize = 6;
}

using EphData = std::array<double, detail::EphDataSize>;

} // namespace specni
