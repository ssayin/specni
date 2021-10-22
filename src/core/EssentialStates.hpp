#pragma once

#include "AccidentalStates.hpp"
#include "Planet.hpp"
#include "core/PlanetPairs.hpp"
#include "swephpp.hpp"
#include <array>
#include <cassert>
#include <core/Cyclic.hpp>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace specni {

static constexpr std::array<swephpp::PlanetaryBody, 12>
    houseOrder /* domicile */ = {
        swephpp::PlanetaryBody::Mars,    swephpp::PlanetaryBody::Venus,
        swephpp::PlanetaryBody::Mercury, swephpp::PlanetaryBody::Moon,
        swephpp::PlanetaryBody::Sun,     swephpp::PlanetaryBody::Mercury,
        swephpp::PlanetaryBody::Venus,   swephpp::PlanetaryBody::Mars,
        swephpp::PlanetaryBody::Jupiter, swephpp::PlanetaryBody::Saturn,
        swephpp::PlanetaryBody::Saturn,  swephpp::PlanetaryBody::Jupiter};

static constexpr std::array<swephpp::PlanetaryBody, 7> faceOrder{
    swephpp::PlanetaryBody::Mars,   swephpp::PlanetaryBody::Sun,
    swephpp::PlanetaryBody::Venus,  swephpp::PlanetaryBody::Mercury,
    swephpp::PlanetaryBody::Moon,   swephpp::PlanetaryBody::Saturn,
    swephpp::PlanetaryBody::Jupiter};

static constexpr std::array<
    std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody>, 12>
    triplicityRulers{
        std::make_tuple(swephpp::PlanetaryBody::Sun,
                        swephpp::PlanetaryBody::Jupiter),
        std::make_tuple(swephpp::PlanetaryBody::Venus,
                        swephpp::PlanetaryBody::Moon),
        std::make_tuple(swephpp::PlanetaryBody::Saturn,
                        swephpp::PlanetaryBody::Mercury),
        std::make_tuple(swephpp::PlanetaryBody::Mars,
                        swephpp::PlanetaryBody::Mars),
        std::make_tuple(swephpp::PlanetaryBody::Sun,
                        swephpp::PlanetaryBody::Jupiter),
        std::make_tuple(swephpp::PlanetaryBody::Venus,
                        swephpp::PlanetaryBody::Moon),
        std::make_tuple(swephpp::PlanetaryBody::Saturn,
                        swephpp::PlanetaryBody::Mercury),
        std::make_tuple(swephpp::PlanetaryBody::Mars,
                        swephpp::PlanetaryBody::Mars),
        std::make_tuple(swephpp::PlanetaryBody::Sun,
                        swephpp::PlanetaryBody::Jupiter),
        std::make_tuple(swephpp::PlanetaryBody::Venus,
                        swephpp::PlanetaryBody::Moon),
        std::make_tuple(swephpp::PlanetaryBody::Saturn,
                        swephpp::PlanetaryBody::Mercury),
        std::make_tuple(swephpp::PlanetaryBody::Mars,
                        swephpp::PlanetaryBody::Mars),
    };

constexpr swephpp::PlanetaryBody face(double lon) {
  return faceOrder.at(std::fmod(lon / 10.0, 7));
}

// should also consider mutual reception
inline bool IsDomicile(const Planet &p) {
  return (p.Id == houseOrder[p.Data.lon / 30.0]);
}

inline bool IsInMutualReceptionDomicile(const Planet &p1, const Planet &p2) {
  return IsDomicile(Planet{p1.Id, p2.Data}) &&
         IsDomicile(Planet{p2.Id, p1.Data});
}

inline bool IsInDetriment(const Planet &p) {
  return (p.Id == houseOrder[static_cast<int>((6 + (p.Data.lon / 30.0))) % 12]);
}

// should consider mutual reception
bool IsExalted(const Planet &p);

bool IsFallen(const Planet &p);

inline bool IsMutualReceptionExalted(const Planet &p1, const Planet &p2) {
  return IsExalted(Planet{p1.Id, p2.Data}) && IsExalted(Planet{p2.Id, p1.Data});
}

enum class EssentialState {
  Domicile,
  MutualDomicile,
  InDetriment,
  Exalted,
  MutualExalted,
  Fallen,
  InOwnTriplicity,
  InOwnFace,
  InOwnTerm,
  Peregrine
};

// determine astrological season
// determine whether it is night or day
// get ruler / participitating ruler
bool IsInOwnTriplicity(const Planet &p);

inline bool IsInOwnFace(const Planet &p) { return (p.Id == face(p.Data.lon)); }

bool IsInOwnTerm(const Planet &p);

const std::unordered_map<Planet, std::vector<EssentialState>>
GetPlanetEssentialStates(std::vector<Planet> &planets, PlanetPairs &pairs);

/*
  Astrological night begins when sun is below Descendant
*/
inline bool IsItNight(Planet &sun, swephpp::Angles &asc) {
  assert(sun.Id == swephpp::PlanetaryBody::Sun);
  return false;
}
} // namespace specni
