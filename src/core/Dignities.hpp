#pragma once

#include "swephpp.hpp"
#include <array>
#include <tuple>

namespace specni {

constexpr std::array<swephpp::PlanetaryBody, 12> houseOrder /* domicile */ {
    swephpp::PlanetaryBody::Mars,    swephpp::PlanetaryBody::Venus,
    swephpp::PlanetaryBody::Mercury, swephpp::PlanetaryBody::Moon,
    swephpp::PlanetaryBody::Sun,     swephpp::PlanetaryBody::Mercury,
    swephpp::PlanetaryBody::Venus,   swephpp::PlanetaryBody::Mars,
    swephpp::PlanetaryBody::Jupiter, swephpp::PlanetaryBody::Saturn,
    swephpp::PlanetaryBody::Saturn,  swephpp::PlanetaryBody::Jupiter};

constexpr std::array<swephpp::PlanetaryBody, 7> faceOrder{
    swephpp::PlanetaryBody::Mars,   swephpp::PlanetaryBody::Sun,
    swephpp::PlanetaryBody::Venus,  swephpp::PlanetaryBody::Mercury,
    swephpp::PlanetaryBody::Moon,   swephpp::PlanetaryBody::Saturn,
    swephpp::PlanetaryBody::Jupiter};

constexpr std::array<std::tuple<swephpp::PlanetaryBody, swephpp::PlanetaryBody>,
                     12>
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
}; // namespace specni