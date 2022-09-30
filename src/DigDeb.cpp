#include "Combinations.hpp"
#include "FastMap.hpp"
#include "SwEphpp.hpp"

#include <numeric>
#include <ranges>

namespace specni::core {

// Aries - Mars, Taurus - Venus and so on.
constexpr std::array<swe::Ipl, 12> houseOrder /* domicile */ {
    swe::Ipl::Mars,    swe::Ipl::Venus,   swe::Ipl::Mercury, swe::Ipl::Moon,
    swe::Ipl::Sun,     swe::Ipl::Mercury, swe::Ipl::Venus,   swe::Ipl::Mars,
    swe::Ipl::Jupiter, swe::Ipl::Saturn,  swe::Ipl::Saturn,  swe::Ipl::Jupiter};

// The faces follow the pattern below
constexpr std::array<swe::Ipl, 7> faceOrder{
    swe::Ipl::Mars, swe::Ipl::Sun,    swe::Ipl::Venus,  swe::Ipl::Mercury,
    swe::Ipl::Moon, swe::Ipl::Saturn, swe::Ipl::Jupiter};

// Starting from the sign of Aries, Day / Night triplicity rulers
constexpr std::array<std::pair<swe::Ipl, swe::Ipl>, 12> triplicityRulers{
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
    std::make_pair(swe::Ipl::Sun, swe::Ipl::Jupiter),
    std::make_pair(swe::Ipl::Venus, swe::Ipl::Moon),
    std::make_pair(swe::Ipl::Saturn, swe::Ipl::Mercury),
    std::make_pair(swe::Ipl::Mars, swe::Ipl::Mars),
};

// Exact degree
constexpr auto exaltations = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 33.0},     // Taurus 3
    {swe::Ipl::TrueNode, 63.0}, // Gemini 3
    {swe::Ipl::Jupiter, 105.0}, // Cancer 15
    {swe::Ipl::Mercury, 195.0}, // Virgo 15
    {swe::Ipl::Saturn, 231.0},  // Libra 21
    {swe::Ipl::Mars, 298.0},    // Aquarius 28
    {swe::Ipl::Venus, 357.0},   // Pisces 27
});

// Upper-bound
using TermsPentad = std::array<std::pair<swe::Ipl, double>, 5>;
constexpr auto terms = std::to_array<TermsPentad>({
    {
        std::make_pair(swe::Ipl::Jupiter, 6.0),
        std::make_pair(swe::Ipl::Venus, 14.0),
        std::make_pair(swe::Ipl::Mercury, 21.0),
        std::make_pair(swe::Ipl::Mars, 26.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 8.0),
        std::make_pair(swe::Ipl::Mercury, 15.0),
        std::make_pair(swe::Ipl::Jupiter, 22.0),
        std::make_pair(swe::Ipl::Saturn, 26.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mercury, 7.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Venus, 21.0),
        std::make_pair(swe::Ipl::Saturn, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mars, 6.0),
        std::make_pair(swe::Ipl::Jupiter, 13.0),
        std::make_pair(swe::Ipl::Mercury, 20.0),
        std::make_pair(swe::Ipl::Venus, 27.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 6.0),
        std::make_pair(swe::Ipl::Mercury, 13.0),
        std::make_pair(swe::Ipl::Venus, 19.0),
        std::make_pair(swe::Ipl::Jupiter, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mercury, 7.0),
        std::make_pair(swe::Ipl::Venus, 13.0),
        std::make_pair(swe::Ipl::Jupiter, 18.0),
        std::make_pair(swe::Ipl::Saturn, 24.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 7.0),
        std::make_pair(swe::Ipl::Venus, 13.0),
        std::make_pair(swe::Ipl::Jupiter, 18.0),
        std::make_pair(swe::Ipl::Mercury, 24.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Mars, 6.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Venus, 21.0),
        std::make_pair(swe::Ipl::Mercury, 27.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Jupiter, 8.0),
        std::make_pair(swe::Ipl::Venus, 14.0),
        std::make_pair(swe::Ipl::Mercury, 19.0),
        std::make_pair(swe::Ipl::Saturn, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 6.0),
        std::make_pair(swe::Ipl::Mercury, 12.0),
        std::make_pair(swe::Ipl::Jupiter, 19.0),
        std::make_pair(swe::Ipl::Mars, 25.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Saturn, 6.0),
        std::make_pair(swe::Ipl::Mercury, 12.0),
        std::make_pair(swe::Ipl::Venus, 20.0),
        std::make_pair(swe::Ipl::Jupiter, 25.0),
        std::make_pair(swe::Ipl::Mars, 30.0),
    },
    {
        std::make_pair(swe::Ipl::Venus, 8.0),
        std::make_pair(swe::Ipl::Jupiter, 14.0),
        std::make_pair(swe::Ipl::Mercury, 20.0),
        std::make_pair(swe::Ipl::Mars, 26.0),
        std::make_pair(swe::Ipl::Saturn, 30.0),
    },
});

constexpr auto slowUB = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 12.30},
    {swe::Ipl::Mercury, 1.00},
    {swe::Ipl::Venus, 0.50},
    {swe::Ipl::Mars, 0.30},
    {swe::Ipl::Jupiter, 0.05},
    {swe::Ipl::Saturn, 0.02},
});

constexpr auto swiftLB = std::to_array<std::pair<swe::Ipl, double>>({
    {swe::Ipl::Moon, 13.30},
    {swe::Ipl::Mercury, 1.38},
    {swe::Ipl::Venus, 1.20},
    {swe::Ipl::Mars, 0.40},
    {swe::Ipl::Jupiter, 0.10},
    {swe::Ipl::Saturn, 0.05},
});

enum class DigDeb : uint32_t {
  None = 0,
  Slow = 1,
  Swift = 2,
  Retrograde = 4,
  Direct = 8,
  Oriental = 16,
  Occidental = 32,
  UnderSunBeams = 64,
  Combust = 128,
  Cazimi = 256,
  FreeFromSunBeams = 512,
  Domicile = 1024,
  Detriment = 2048,
  InOwnTriplicity = 4096,
  InOwnFace = 8192,
  InOwnTerm = 16384,
  Exalted = 32768,
  Fallen = 65536,
};

constexpr DigDeb operator|(DigDeb a, DigDeb b) {
  return static_cast<DigDeb>(static_cast<std::underlying_type_t<DigDeb>>(a) |
                             static_cast<std::underlying_type_t<DigDeb>>(b));
}

constexpr DigDeb operator|=(DigDeb &a, DigDeb b) {
  a = a | b;
  return a;
}

constexpr DigDeb operator&(DigDeb a, DigDeb b) {
  return static_cast<DigDeb>(static_cast<std::underlying_type_t<DigDeb>>(a) &
                             static_cast<std::underlying_type_t<DigDeb>>(b));
}

constexpr std::string_view to_string(DigDeb dd) {
  switch (dd) {
    using enum DigDeb;
  case Swift:
    return "Swift";
  case Slow:
    return "Slow";
  case Retrograde:
    return "Retrograde";
  case Direct:
    return "Direct";
  case Oriental:
    return "Oriental";
  case Occidental:
    return "Occidental";
  case UnderSunBeams:
    return "UnderSunBeams";
  case Combust:
    return "Combust";
  case Cazimi:
    return "Cazimi";
  case FreeFromSunBeams:
    return "FreeFromSunBeams";
  case Domicile:
    return "Domicile";
  case Detriment:
    return "Detriment";
  case InOwnTriplicity:
    return "InOwnTriplicity";
  case InOwnFace:
    return "InOwnFace";
  case InOwnTerm:
    return "InOwnTerm";
  case Exalted:
    return "Exalted";
  case Fallen:
    return "Fallen";
  default:
    throw std::invalid_argument("invalid DigDeb for to_string(...)");
  }
}

auto is_peregrine(DigDeb dd) { return dd < DigDeb::Domicile; }

// template <typename T, T t, typename Comp>
template <auto t, typename Comp>
auto fastMapCmp(swe::Ipl ipl, double x, Comp cmp) {
  static constexpr auto map = FastMap<swe::Ipl, double, t.size()>{{t}};
  if (auto found = map.at(ipl)) {
    return cmp(x, *found);
  } else {
    return false;
  }
}

std::size_t to_sign_index(double lon) {
  return static_cast<std::size_t>(lon / 30);
}

using DigDebCb = std::pair<DigDeb, std::function<bool(const swe::Planet &)>>;
using DigDebCb2 = std::pair<DigDeb, std::function<bool(swe::Ipl, double lon)>>;

const auto digDebTable2 = std::to_array<DigDebCb2>({
    {DigDeb::Domicile,
     [](swe::Ipl id, double lon) {
       return id == houseOrder.at(to_sign_index(lon));
     }},
    {DigDeb::Detriment,
     [](swe::Ipl id, double lon) {
       return id == houseOrder.at((to_sign_index(lon) + 6) % 12);
     }},
    {DigDeb::InOwnFace,
     [](swe::Ipl id, double lon) {
       return id ==
              core::faceOrder.at(static_cast<std::size_t>(lon / 10.0) % 7);
     }},
    {DigDeb::InOwnTerm,
     [](swe::Ipl id, double lon) {
       const auto d = std::fmod(lon, 30);
       const TermsPentad &sign = terms.at(to_sign_index(lon));
       return std::ranges::adjacent_find(
                  sign, [&](const auto &x, const auto &y) {
                    return d >= x.second && d < y.second && y.first == id;
                  }) != sign.end();
     }},
    {DigDeb::Exalted,
     [](swe::Ipl id, double lon) {
       return fastMapCmp<core::exaltations>(id, lon, std::equal_to<>{});
     }},
    {DigDeb::Fallen,
     [](swe::Ipl id, double lon) {
       return fastMapCmp<core::exaltations>(id, swe_degnorm(lon + 180.0),
                                            std::equal_to<>{});
     }},
});

const auto digDebCallTable = std::to_array<DigDebCb>({
    {DigDeb::Swift,
     [](const auto &pl) {
       return fastMapCmp<swiftLB>(pl.id(), pl.ecliptic().at(3),
                                  std::greater_equal<>{});
     }},
    {DigDeb::Slow,
     [](const auto &pl) {
       return fastMapCmp<slowUB>(pl.id(), pl.ecliptic().at(3),
                                 std::less_equal<>{});
     }},
    {DigDeb::Retrograde,
     [](const auto &pl) {
       return pl.id() != swe::Ipl::Sun && pl.id() != swe::Ipl::Moon &&
              pl.ecliptic().at(3) < 0.0;
     }},
    {DigDeb::Direct,
     [](const auto &pl) {
       return pl.id() != swe::Ipl::Sun && pl.id() != swe::Ipl::Moon &&
              pl.ecliptic().at(3) > 0.0;
     }},
});

consteval auto dms2double(double degs, double mins, double secs) {
  return degs + mins / 60.0 + secs / 3600.0;
}

constexpr auto orbAbs(const auto &a, const auto &b) {
  return std::abs(swe_difdeg2n(a.ecliptic().at(0), b.ecliptic().at(0)));
}

DigDeb sunAccDeb(const auto &sun, const auto &a) {
  constinit static auto orbCombust = dms2double(8, 30, 0);
  constinit static auto orbUnderSunBeams = dms2double(17, 0, 0);
  constinit static auto orbCazimi = dms2double(0, 17, 0);
  const auto orb = orbAbs(sun, a);
  if (orb > orbUnderSunBeams)
    return DigDeb::FreeFromSunBeams;
  if (orb <= orbCazimi)
    return DigDeb::Cazimi;
  if (orb <= orbCombust)
    return DigDeb::Combust;

  return DigDeb::UnderSunBeams;
}

auto houseScore(std::size_t house_num) {
  constinit static auto _ =
      std::to_array<char>({0, 5, 3, 1, 4, 3, -2, 4, -2, 2, 5, 4, -5});
  assert(house_num >= 0 && house_num <= 12);
  return _.at(house_num);
}

/*
auto sunPlanetRise(const Chart &c, const auto &a) {
  double tret[3];
  double tret2[3];

  double geo[3] = {c.coord.at(0), c.coord.at(1), 0.0};

  swe::swe_call(swe_rise_trans, c.ut,
                static_cast<std::underlying_type_t<swe::Ipl>>(a.id()), nullptr,
                0, SE_CALC_RISE, geo, 0, 0, tret);

  swe::swe_call(swe_rise_trans, c.ut,
                static_cast<std::underlying_type_t<swe::Ipl>>(swe::Ipl::Sun),
                nullptr, 0, SE_CALC_RISE, geo, 0, 0, tret2);

  return tret[0] < tret2[0] ? DigDeb::Oriental : DigDeb::Occidental;
}
*/

auto isInOwnTriplicity(bool is_night, const auto &a) {
  const auto &rulers = triplicityRulers.at(to_sign_index(a.ecliptic().at(0)));
  return (is_night ? std::get<1>(rulers) : std::get<0>(rulers)) == a.id();
}

// run this only with peregrine
void digDebByReception(core::swe::Planet &p1, core::swe::Planet &p2) {
  std::vector<core::DigDebCb2> a{core::digDebTable2.begin(),
                                 core::digDebTable2.end()};
  for_each_combination(
      a.begin(), a.begin() + 2, a.end(), [&p1, &p2](auto &b, auto &e) {
        if (b->second(p1.id(), p2.ecliptic().at(0)) &&
            std::next(b)->second(p2.id(), p1.ecliptic().at(0))) {
          std::cout << p1.name() << " " << to_string(b->first) << " "
                    << p2.name() << " " << to_string(std::next(b)->first)
                    << std::endl;
        }

        return false;
      });
}

template <std::ranges::range R> void digDebByReception(R &&r) {
  for_each_combination(std::ranges::begin(r), std::ranges::begin(r) + 2,
                       std::ranges::end(r), [](auto &b, auto &e) {
                         digDebByReception(*b, *std::next(b));
                         return false;
                       });
}

auto digDebOf(const swe::Planet &pl) {
  auto digdeb1 = [](const core::swe::Planet &pl) {
    return std::transform_reduce(
        core::digDebCallTable.begin(), core::digDebCallTable.end(),
        core::DigDeb::None, std::bit_or<>{}, [&pl](auto x) {
          if (x.second(pl))
            return x.first;
          else
            return core::DigDeb::None;
        });
  };

  auto digdeb2 = [](const core::swe::Planet &pl) {
    return std::transform_reduce(core::digDebTable2.begin(),
                                 core::digDebTable2.end(), core::DigDeb::None,
                                 std::bit_or<>{}, [&pl](auto x) {
                                   if (x.second(pl.id(), pl.ecliptic().at(0)))
                                     return x.first;
                                   else
                                     return core::DigDeb::None;
                                 });
  };

  return digdeb1(pl) | digdeb2(pl);
}

template <std::ranges::range R> auto digDebPlanets(R &&r, double ac) {
  auto sun = std::ranges::find_if(
      r, [](const auto &pl) { return pl.id() == core::swe::Ipl::Sun; });

  std::ranges::for_each(r, [&](const core::swe::Planet &pl) {
    auto x = digDebOf(pl);
    if (sun != std::end(r) && sun->id() != pl.id()) {
      if (auto is_night =
              [](const auto &sun, auto ac) {
                return swe_difdeg2n(sun.ecliptic().at(0), ac) >= 0;
              }(*sun, ac);
          core::isInOwnTriplicity(is_night, pl)) {
        x |= DigDeb::InOwnTriplicity;
      }
      x |= core::sunAccDeb(*sun, pl);
      // x |= core::sunPlanetRise(model, pl);
    }
    //
  });
}

} // namespace specni::core
