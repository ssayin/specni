#pragma once

#include "Cyclic.hpp"

namespace specni {

struct HasLongitude {
	[[nodiscard]] virtual Longitude longitude() const = 0;
};

struct HasLatitude {
	[[nodiscard]] virtual double latitude() const = 0;
};

struct HasEcliptic: public HasLatitude, HasLongitude { };

struct HasDeclination {
	[[nodiscard]] virtual Declination declination() const = 0;
};

} // namespace specni
