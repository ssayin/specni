#pragma once

// clang-format off
#include "swephexp.h"
// clang-format on

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

namespace specni::core::swe {

enum class ZodiacType : int32_t { Tropical = 0, Sidereal, Radians };

enum class HouseSystem : char {
  Equal = 'A',
  Alcabitius = 'B',
  Campanus = 'C',
  EqualMC = 'D',
  Carter = 'F',
  Gauquelin = 'G',
  Azimuth = 'H',
  Sunshine = 'I',
  SunshineAlt = 'J',
  Koch = 'K',
  PullenSDelta = 'L',
  Morinus = 'M',
  WholeSign = 'N',
  Porphyry = 'O',
  Placidus = 'P',
  PullenSRatio = 'Q',
  Regiomontanus = 'R',
  Sripati = 'S',
  PolichPage = 'T',
  KrusinskiPisaGoelzer = 'U',
  EqualVehlow = 'V',
  EqualWholeSign = 'W',
  ARSMeridian = 'X',
  APC = 'X'
};

enum class Ipl : int32_t {
  Sun = 0,
  Moon = 1,
  Mercury = 2,
  Venus = 3,
  Mars = 4,
  Jupiter = 5,
  Saturn = 6,
  Uranus = 7,
  Neptune = 8,
  Pluto = 9,
  MeanNode = 10,
  TrueNode = 11,
  MeanApogee = 12,
  OsculatingApogee = 13,
  Earth = 14,
  Chiron = 15,
  Pholus = 16,
  Ceres = 17,
  Pallas = 18,
  Juno = 19,
  Vesta = 20,
  InterpolatedApogee = 21,
  InterpolatedPerigee = 22,
  /* for Uranian astrology */
  Cupido = 40,
  Hades = 41,
  Zeus = 42,
  Kronos = 43,
  Apollon = 44,
  Admetos = 45,
  Vulkanus = 46,
  Poseidon = 47,
  Isis = 48,
  Nibiru = 49,
  Harrington = 50,
  NeptuneLeverrier = 51,
  NeptuneAdams = 52,
  PlutoLowell = 53,
  PlutoPickering = 54,
  Vulcan = 55,
  WhiteMoon = 56,
  Proserpina = 57,
  Waldemath = 58,
};

enum class EphFlag : int32_t {
  JPLEph = 1,
  SwissEph = 1 << 1,
  MoshierEph = 1 << 2,
  Heliocentric = 1 << 3,
  TruePosition = 1 << 4,
  J2000 = 1 << 5,
  NoNutation = 1 << 6,
  Speed3 = 1 << 7,
  Speed = 1 << 8,
  NoGDeflection = 1 << 9,
  NoAberration = 1 << 10,
  Astrometric = NoAberration | NoGDeflection,
  Equatorial = 1 << 11,
  XYZ = 1 << 12,
  Radians = 1 << 13,
  Barycentric = 1 << 14,
  Topocentric = 1 << 15,
  OrbelAA = Topocentric,
  Sidereal = 1 << 16,
  ICRS = 1 << 17,
  DPSIDEPS1980 = 1 << 18,
  JPLHorizons = DPSIDEPS1980,
  JPLHorizonsApprox = 1 << 19
};

constexpr auto operator|(const EphFlag &lhs, const EphFlag &rhs) {
  return static_cast<EphFlag>(
      static_cast<std::underlying_type_t<EphFlag>>(lhs) |
      static_cast<std::underlying_type_t<EphFlag>>(rhs));
}

static constexpr auto AnglesSize = 10;
using Angles = std::array<double, AnglesSize>;

enum Angle {
  AC,
  MC,
  ArmC,
  Vertex,
  EquatorialAC,
  CoACKoch,
  CoACMunkasey,
  PolarACMunkasey,
};

using Coordinate = std::array<double, 2>;

enum class Coord { Latitude = 0, Longitude };

template <typename T, typename... Ts> decltype(auto) swe_call(T t, Ts &&...ts) {
  using SweErrStr = std::array<char, 256>;
  SweErrStr serr;
  auto x = t(std::forward<Ts>(ts)..., serr.data());
  std::cout << serr.data();
  return x;
}

struct Context {
  Context() { swe_set_ephe_path(const_cast<char *>("ephe")); }
  explicit Context(std::string eph_path) { swe_set_ephe_path(eph_path.data()); }
  Context(Context &) = delete;
  Context &operator=(const Context &) = delete;
  Context const &operator=(Context &&) = delete;
  Context(Context &&) = delete;
  ~Context() { swe_close(); }
};

struct ContextInit {
  ContextInit() { static Context ctx; }
};

namespace detail {
static constexpr uint8_t EphDataSize = 6;
}

using EphData = std::array<double, detail::EphDataSize>;

struct DateTime {
  int Year;
  unsigned Month;
  unsigned Day;
  unsigned Hours;
  unsigned Minutes;
  unsigned Seconds;
};

class Ut : ContextInit {
  static constexpr int GregorianFlag = 1;
  static constexpr int UT1 = 1;

public:
  Ut(Ut &&) = default;

  Ut(const DateTime &dt) {
    assert(swe_call(swe_utc_to_jd, dt.Year, dt.Month, dt.Day, dt.Hours,
                    dt.Minutes, dt.Seconds, GregorianFlag, dret.data()) == OK);
  }

  operator double() const { return dret[UT1]; }

private:
  std::array<double, 2> dret{};
};

template <typename T>
concept HasEcliptic = requires(T t) {
  t.ecliptic();
};

template <typename T>
concept HasEquatorial = requires(T t) {
  t.equatorial();
};

class House : ContextInit {
public:
  House(const Ut &ut, Coordinate geodetic, HouseSystem hsys)
      : geodetic{geodetic}, hsys{hsys} {
    update(ut, geodetic, hsys);
  }

  void update(const Ut &ut, Coordinate geodetic, HouseSystem hsys) {
    static constexpr std::size_t GauquelinSize = 37;
    static constexpr std::size_t DefaultSize = 13;

    this->hsys = hsys;

    hsys == HouseSystem::Gauquelin ? cusps.resize(GauquelinSize)
                                   : cusps.resize(DefaultSize);

    swe_houses(ut, geodetic.at(static_cast<std::size_t>(Coord::Latitude)),
               geodetic.at(static_cast<std::size_t>(Coord::Longitude)),
               static_cast<std::underlying_type_t<HouseSystem>>(hsys),
               &cusps[0], reinterpret_cast<double *>(&ang));

    cusps.shrink_to_fit();

    nutation = [&]() {
      EphData data;
      swe_call(swe_calc_ut, ut, SE_ECL_NUT,
               static_cast<std::underlying_type_t<EphFlag>>(EphFlag::SwissEph),
               data.data());
      return data.at(0);
    }();
  }

  template <HasEcliptic EC> double housePos(const EC &ec) const {
    return swe_call(
        swe_house_pos, ang.at(ArmC),
        geodetic.at(static_cast<std::size_t>(Coord::Latitude)), nutation,
        static_cast<std::underlying_type_t<HouseSystem>>(hsys),
        std::to_array<double>({ec.ecliptic().at(0), ec.ecliptic().at(1)})
            .data());
  }

  Angles ang{0};
  std::vector<double> cusps;

private:
  Coordinate geodetic;
  HouseSystem hsys;
  double nutation;
};

class Planet : ContextInit {
public:
  enum { Ecliptic = 0, Equatorial };
  Planet(Ipl ipl, const Ut &ut, EphFlag flag) : ipl(ipl) {
    nm.resize(SE_MAX_STNAME);
    swe_get_planet_name(static_cast<std::underlying_type_t<Ipl>>(ipl), &nm[0]);
    nm.shrink_to_fit();
    update(ut, flag);
  }

  auto id() const -> Ipl { return ipl; };
  auto name() const { return nm; }

  auto operator<=>(const Planet &o) const { return (o.ipl <=> this->ipl); }

  const auto &ecliptic() const { return eph.at(0); }
  const auto &equatorial() const { return eph.at(1); }

  void update(const Ut &ut, EphFlag flag) {
    auto init_data = [&](EphFlag fl, std::size_t index) {
      swe_call(swe_calc_ut, ut, static_cast<std::underlying_type_t<Ipl>>(ipl),
               static_cast<std::underlying_type_t<EphFlag>>(fl),
               eph.at(index).data());
    };

    init_data(flag, 0);
    init_data(flag | EphFlag::Equatorial, 1);
  }

private:
  Ipl ipl;
  std::array<EphData, 2> eph;
  std::string nm;
};

class FixedStar : ContextInit {
public:
  FixedStar(std::string id, const Ut &ut, EphFlag flag) : id(std::move(id)) {
    update(ut, flag);
  }

  void update(const Ut &ut, EphFlag flag) {
    swe_call(swe_fixstar_ut, id.data(), static_cast<double>(ut),
             static_cast<std::underlying_type_t<EphFlag>>(flag), ecl.data());
  }

  const auto &ecliptic() const { return ecl; }

private:
  std::string id;
  EphData ecl{0};
};

}; // namespace specni::core::swe

namespace std {
template <> struct hash<specni::core::swe::Planet> {
  auto operator()(const specni::core::swe::Planet &p) const -> std::size_t {
    return std::hash<specni::core::swe::Ipl>()(p.id());
  }
};
} // namespace std
