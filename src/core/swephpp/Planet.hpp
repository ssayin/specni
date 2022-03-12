#pragma once

#include "Base.hpp"
#include "Context.hpp"
#include "Cyclic.hpp"
#include "Defs.hpp"
#include "EphData.hpp"
#include "Ut.hpp"

namespace specni::core::swe {

class Planet: ContextInit,
			  public HasEcliptic,
			  public HasDeclination {
	friend std::ostream& operator<<(std::ostream& out, Planet const& p);

public:
	Planet(const Ipl& ipl, const Ut& ut, const EphFlag& flag);

	[[nodiscard]] auto id() const -> Ipl { return ipl; };

	[[nodiscard]] auto name() const -> std::string;

	[[nodiscard]] auto longitude() const -> Longitude override { return Longitude{ecliptic[0]}; }

	[[nodiscard]] auto latitude() const -> double override { return ecliptic[1]; }

	[[nodiscard]] auto distance() const -> double { return ecliptic[2]; }

	[[nodiscard]] auto speedLongitude() const -> Longitude { return Longitude{ecliptic[3]}; }

	[[nodiscard]] auto speedLatitude() const -> double { return ecliptic[4]; }

	[[nodiscard]] auto speedDistance() const -> double { return ecliptic[5]; }

	[[nodiscard]] auto declination() const -> Declination override { return Declination{equatorial[1]}; }

	auto operator==(const Planet& o) const -> bool
	{
		return (o.ipl==this->ipl);
	}

private:
	Ipl ipl;
	EphData ecliptic{0};
	EphData equatorial{0};
};

std::ostream& operator<<(std::ostream& out, Planet const& p);

} // namespace specni

namespace std {
template<>
struct hash<specni::core::swe::Planet> {
	auto operator()(const specni::core::swe::Planet& p) const -> std::size_t
	{
		return std::hash<specni::core::swe::Ipl>()(p.id());
	}
};
} // namespace std
