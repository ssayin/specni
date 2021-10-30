#pragma once

#include "swephpp.hpp"
#include <array>
#include <tuple>

namespace specni {

constexpr std::array<swephpp::Ipl, 12> houseOrder /* domicile */ {
    swephpp::Ipl::Mars,   swephpp::Ipl::Venus,  swephpp::Ipl::Mercury,
    swephpp::Ipl::Moon,   swephpp::Ipl::Sun,    swephpp::Ipl::Mercury,
    swephpp::Ipl::Venus,  swephpp::Ipl::Mars,   swephpp::Ipl::Jupiter,
    swephpp::Ipl::Saturn, swephpp::Ipl::Saturn, swephpp::Ipl::Jupiter};

constexpr std::array<swephpp::Ipl, 7> faceOrder{
    swephpp::Ipl::Mars,    swephpp::Ipl::Sun,  swephpp::Ipl::Venus,
    swephpp::Ipl::Mercury, swephpp::Ipl::Moon, swephpp::Ipl::Saturn,
    swephpp::Ipl::Jupiter};

constexpr std::array<std::tuple<swephpp::Ipl, swephpp::Ipl>, 12>
    triplicityRulers{
        std::make_tuple(swephpp::Ipl::Sun, swephpp::Ipl::Jupiter),
        std::make_tuple(swephpp::Ipl::Venus, swephpp::Ipl::Moon),
        std::make_tuple(swephpp::Ipl::Saturn, swephpp::Ipl::Mercury),
        std::make_tuple(swephpp::Ipl::Mars, swephpp::Ipl::Mars),
        std::make_tuple(swephpp::Ipl::Sun, swephpp::Ipl::Jupiter),
        std::make_tuple(swephpp::Ipl::Venus, swephpp::Ipl::Moon),
        std::make_tuple(swephpp::Ipl::Saturn, swephpp::Ipl::Mercury),
        std::make_tuple(swephpp::Ipl::Mars, swephpp::Ipl::Mars),
        std::make_tuple(swephpp::Ipl::Sun, swephpp::Ipl::Jupiter),
        std::make_tuple(swephpp::Ipl::Venus, swephpp::Ipl::Moon),
        std::make_tuple(swephpp::Ipl::Saturn, swephpp::Ipl::Mercury),
        std::make_tuple(swephpp::Ipl::Mars, swephpp::Ipl::Mars),
    };
}; // namespace specni