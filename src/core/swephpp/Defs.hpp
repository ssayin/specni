#pragma once

#include <array>
#include <cstdint>

namespace specni {
namespace core {
namespace swe {

using ErrStr = char[256];

enum class ZodiacType: int32_t {
	Tropical = 0, Sidereal, Radians
};

enum class HouseSystem: char {
	Equal = 'A',
	Alcabitius,
	Campanus,
	EqualMC,
	Carter = 'F',
	Gauquelin,
	Azimuth = 'H',
	Sunshine,
	SunshineAlt,
	Koch,
	PullenSDelta,
	Morinus,
	WholeSign,
	Porphyry,
	Placidus,
	PullenSRatio,
	Regiomontanus,
	Sripati,
	PolichPage,
	KrusinskiPisaGoelzer,
	EqualVehlow,
	EqualWholeSign,
	ARSMeridian,
	APC
};

enum class Ipl: int32_t {
	Sun,
	Moon,
	Mercury,
	Venus,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Pluto,
	MeanNode,
	TrueNode,
	MeanApogee,
	OsculatingApogee,
	Earth,
	Chiron,
	Pholus,
	Ceres,
	Pallas,
	Juno,
	Vesta,
	InterpolatedApogee,
	InterpolatedPerigee,
	/* for Uranian astrology */
	Cupido = 40,
	Hades,
	Zeus,
	Kronos,
	Apollon,
	Admetos,
	Vulkanus,
	Poseidon,
	Isis,
	Nibiru,
	Harrington,
	NeptuneLeverrier,
	NeptuneAdams,
	PlutoLowell,
	PlutoPickering,
	Vulcan,
	WhiteMoon,
	Proserpina,
	Waldemath,
};

enum class EphFlag: int32_t {
	JPLEph = 1,
	SwissEph = 2,
	MoshierEph = 4,
	Heliocentric = 8,
	TruePosition = 16,
	J2000 = 32,
	NoNutation = 64,
	Speed3 = 128,
	Speed = 256,
	NoGDeflection = 512,
	NoAberration = 1024,
	Astrometric = NoAberration | NoGDeflection,
	Equatorial = 2*1024,
	XYZ = 4*1024,
	Radians = 8*1024,
	Barycentric = 16*1024,
	Topocentric = 32*1024,
	OrbelAA = Topocentric,
	Sidereal = 64*1024,
	ICRS = 128*1024,
	DPSIDEPS1980 = 256*1024,
	JPLHorizons = DPSIDEPS1980,
	JPLHorizonsApprox = 512*1024
};

constexpr auto operator|(const EphFlag& lhs, const EphFlag& rhs)

-> EphFlag
{
	return static_cast<EphFlag>(
			static_cast
					<std::underlying_type_t<EphFlag>>(lhs)
					|
							static_cast
									<std::underlying_type_t<EphFlag>>(rhs)
	);
}

}; // namespace swe
}; // namespace core
}; // namespace specni
