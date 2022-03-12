#ifndef SPECNI_WHEEL_H
#define SPECNI_WHEEL_H

#include <array>
#include "imgui.h"
#include "ChartSettings.h"

#include <core/Chart.hpp>

namespace specni::gui {

class Wheel {
public:
	Wheel(ChartSettings settings,  core::Chart& model);
	void Show() const;

private:
	ChartSettings settings;
	core::Chart model;
	void drawCircles(ImDrawList* draw_list, const ImVec2& window_center,
			const std::array<ImVec2, ChartSettings::Count>& RPoints) const;
	[[nodiscard]] std::array<ImVec2, 5> getRPoints(const ImVec2& window_size) const;
	void drawSignGlyphs(ImDrawList* draw_list, const ImVec2& window_center, const std::array<ImVec2, 5>& RPoints,
			const ImVec2& RSign) const;
	void drawPlanetGlyphs(ImDrawList* draw_list, const ImVec2& window_size, const ImVec2& window_center) const;
};
}

#endif //SPECNI_WHEEL_H