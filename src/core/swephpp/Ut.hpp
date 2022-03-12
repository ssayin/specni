#pragma once

#include "Context.hpp"
#include <array>
#include <cstdint>

namespace specni::core::swe {

struct GregorianTime {
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
};

class Ut: ContextInit {
public:
	Ut(Ut&&) = default;

	Ut(const GregorianTime& dt);

	operator double() const { return m; }

private:
	double m;
};

} // namespace specni
