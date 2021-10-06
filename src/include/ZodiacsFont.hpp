#pragma once

#include "swephpp.hpp"
#include <unordered_map>

static const std::unordered_map<swephpp::PlanetaryBody, char> PlanetCharMap = {
    {swephpp::PlanetaryBody::Sun, 'A'},
    {swephpp::PlanetaryBody::Mercury, 'B'},
    {swephpp::PlanetaryBody::Moon, 'C'},
    {swephpp::PlanetaryBody::Venus, 'D'},
    {swephpp::PlanetaryBody::Mars, 'E'},
    {swephpp::PlanetaryBody::Jupiter, 'F'},
    {swephpp::PlanetaryBody::Saturn, 'G'},
    {swephpp::PlanetaryBody::Uranus, 'H'},
    {swephpp::PlanetaryBody::Neptune, 'I'},
    {swephpp::PlanetaryBody::Pluto, 'J'},
    {swephpp::PlanetaryBody::TrueNode, 'M'},
    {swephpp::PlanetaryBody::Chiron, 'Q'},
    {swephpp::PlanetaryBody::Pholus, 'R'},
    {swephpp::PlanetaryBody::Ceres, 'S'},
    {swephpp::PlanetaryBody::Pallas, 'T'},
    {swephpp::PlanetaryBody::Juno, 'U'},
    {swephpp::PlanetaryBody::Vesta, 'V'},
};