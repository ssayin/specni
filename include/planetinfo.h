#pragma once

#include <string>

class planet_info {
    public:
        planet_info();
        virtual ~planet_info();

        const std::string get_sign();
        const std::string get_degrees();

        void set_ut(double ut);
        double get_ut();

        void set_planetid(int id);

        const std::string get_planet_name();

        void calc();
    private:
    typedef struct {
        double lng;
        double lat;
        double dist;
        double spd_lng;
        double spd_lat;
        double spd_dist;
    } planet_eph_data;

    double ut;
    int id;
    planet_eph_data data;
};
