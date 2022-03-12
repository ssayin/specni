//
// Created by ssayin on 3/12/22.
//

#ifndef SPECNI_CHARTSETTINGS_H
#define SPECNI_CHARTSETTINGS_H

#include <array>
#include <core/Chart.hpp>
#include "imgui.h"
namespace specni::gui {

struct ChartSettings {
	ImColor BaseColor = {};
	ImColor SignColor = {};
	ImColor PlanetColor = {};
	ImColor AscMcColor = {};
	enum Ratio {
		Innermost,
		SignInner,
		SignOuter,
		CuspText,
		CircleHouseNumbers,
		Count
	};

	static constexpr std::array<double, Count> Ratios = {0.25f, 0.40f, 0.45f, 0.40f, 0.28f};

	static constexpr float Thickness = 1.0f;
	ImFont *font;

	explicit ChartSettings(ImFont *font)
			: BaseColor(ImVec4(0.5f, 1.f, 0.5f, 1.f)),
			  SignColor(ImVec4(1, 1, 0.5, 1)), PlanetColor(ImColor(25, 255, 255)),
			  AscMcColor(ImVec4(0.5, 1, 1, 1)), font(font) {}
};

}


#endif //SPECNI_CHARTSETTINGS_H
