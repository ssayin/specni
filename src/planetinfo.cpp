#include "planetinfo.h"

#include "swe/swephexp.h"

#include <sstream>

planet_info::planet_info() {

}


planet_info::~planet_info() {

}


const std::string planet_info::get_sign() {
    const char* signs[] = {
        "Aries",
        "Taurus",
        "Gemini",
        "Cancer",
        "Leo",
        "Virgo",
        "Libra",
        "Scorpio",
        "Sagittarius",
        "Capricorn",
        "Aquarius",
        "Pisces",
    };

    double pos = this->data.lng;
    int sign = pos / 30.0;
    
    return signs[sign];
}


const std::string planet_info::get_degrees() {
    double pos = this->data.lng;
    while (pos >= 30) {
        pos -= 30.0;
    }

    std::stringstream ss;
    ss << pos;
    return ss.str();
}


void planet_info::set_ut(double ut) {
    this->ut = ut;
}


double planet_info::get_ut() {
    return ut;
}


void planet_info::set_planetid(int id) {
    this->id = id;
}


void planet_info::calc() {
    swe_calc_ut(ut, id, SEFLG_SWIEPH, (double*)&data, 0);
}


const std::string planet_info::get_planet_name() {
    switch (id) {
        case SE_SUN:
            return "Sun";
        case SE_MERCURY:
            return "Mercury";
        case SE_MOON:
            return "Moon";
        case SE_VENUS:
            return "Venus";
        case SE_MARS:
            return "Mars";
        case SE_JUPITER:
            return "Jupiter";
        case SE_SATURN:
            return "Saturn";
        case SE_URANUS:
            return "Uranus";
        case SE_NEPTUNE:
            return "Neptune";
        case SE_CHIRON:
            return "Chiron";
        case SE_PLUTO:
            return "Pluto";
        default:
            return "UNKNOWN";
    }
}