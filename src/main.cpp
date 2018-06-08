#include "swe/swephexp.h"
#include "planetinfo.h"

#include <iostream>


int main(int argc, char** argv) {
    double ut = swe_julday(2018, 6, 7, 10, 1);

    planet_info* pi = new planet_info();
    pi->set_ut(ut);
    for (int i = 0; i < 10; ++i) {
        pi->set_planetid(i);
        pi->calc();
        std::cout << pi->get_planet_name() << " " << pi->get_sign() << " " << pi->get_degrees()
            << std::endl;
    }

    return 0;
}