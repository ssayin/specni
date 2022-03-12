
#ifndef SPECNI_MATH_H
#define SPECNI_MATH_H
#include <cmath>
#include <tuple>
#include <core/swephpp.hpp>

namespace specni::gui::degree {

template <typename T>
constexpr T toRadians(T deg) {
	return deg * (M_PI / 180);
}

template <typename T>
constexpr std::tuple<T, T> cosSin(T deg) {
	return std::make_tuple(
			cosf(toRadians(-deg)),
			sinf(toRadians(-deg))); // we want to draw counter-clockwise
}


std::tuple<double, double>
cosSinFixAt(double deg, double asc) {
	return cosSin(deg - asc - 90);
}

}



#endif //SPECNI_MATH_H
