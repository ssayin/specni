#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

// clang-format off
#include "swephexp.h"
// clang-format on

namespace swephpp {

enum class HouseCuspFlag : int32_t { Tropical = 0, Sidereal, Radians };
enum class HouseSystem : char {
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

typedef std::array<double, 13> HouseCusps;
typedef std::array<double, 37> GauquelinCusps;

typedef struct {
  double ac;
  double mc;
  double armc;
  double vertex;
  double equasc; /* equatorial ascendant */
  double coasc1; /* co-ascendant (W. Koch) */
  double coasc2; /* co-ascendant (M. Munkasey)*/
  double polasc; /* polar ascendant (M. Munkasey)*/
  double res1;
  double res2;
} Angles;

typedef struct {
  double lon;
  double lat;
  double dist;
  double spdlon;
  double spdlat;
  double spddist;
} PlanetEphData;

enum class PlanetaryBody : int32_t {
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

enum class Flag : int32_t {
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
  Equatorial = 2 * 1024,
  XYZ = 4 * 1024,
  Radians = 8 * 1024,
  Barycentric = 16 * 1024,
  Topocentric = 32 * 1024,
  OrbelAA = Topocentric,
  Sidereal = 64 * 1024,
  ICRS = 128 * 1024,
  DPSIDEPS1980 = 256 * 1024,
  JPLHorizons = DPSIDEPS1980,
  JPLHorizonsApprox = 512 * 1024
};

inline constexpr Flag operator|(const Flag &lhs, const Flag &rhs) {
  return static_cast<Flag>(static_cast<int32_t>(lhs) |
                           static_cast<int32_t>(rhs));
}

typedef struct {
  double jd_ut;
  int32_t id;
  Flag flag;
} CalcOpts;

typedef struct {
  double jd_ut;
  HouseCuspFlag flag;
  double geolat;
  double geolon;
  HouseSystem hsys;
} HouseOpts;

template <typename T>
inline int houses_ex(const HouseOpts &opts, T &cusps, Angles &ascmc) {
  return swe_houses_ex(opts.jd_ut, static_cast<int32_t>(opts.flag), opts.geolat,
                       opts.geolon, static_cast<char>(opts.hsys),
                       reinterpret_cast<double *>(&cusps),
                       reinterpret_cast<double *>(&ascmc));
}

inline const std::string house_name(HouseSystem sys) {
  return swe_house_name(static_cast<char>(sys));
}

inline int calc(const CalcOpts &opts, PlanetEphData &data) {
  return swe_calc_ut(opts.jd_ut, opts.id, static_cast<int32_t>(opts.flag),
                     reinterpret_cast<double *>(&data), 0);
}

inline const std::string planet_name(PlanetaryBody body) {
  char n[SE_MAX_STNAME];
  swe_get_planet_name(static_cast<int>(body), n);
  return std::string(n);
}

inline int fixstar(double ut, char *name, Flag flg, double xx[6]) {
  char err[256];
  int ret = swe_fixstar_ut(name, ut, static_cast<int32_t>(flg), xx, err);
  std::cerr << err << std::endl;
  return ret;
}

} // namespace swephpp
